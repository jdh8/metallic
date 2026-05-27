#include "FILE.h"
#include "getc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>

#define BUFSZ 128

struct Scanner
{
    FILE* stream;
    size_t count;
    int peek;
    int eof;
};

static int sgetc(struct Scanner* s)
{
    if (s->peek >= 0) {
        int c = s->peek;
        s->peek = -1;
        ++s->count;
        return c;
    }
    if (s->eof)
        return EOF;
    int c = getc_(s->stream);
    if (c == EOF) {
        s->eof = 1;
        return EOF;
    }
    ++s->count;
    return c;
}

static void sungetc(struct Scanner* s, int c)
{
    if (c == EOF)
        return;
    s->peek = c;
    --s->count;
}

static void skip_ws(struct Scanner* s)
{
    int c;
    do { c = sgetc(s); } while (c != EOF && isspace(c));
    sungetc(s, c);
}

static unsigned length_(const char* p)
{
    switch (*p) {
        case 'h': return 'h' << 2 | (1 + (p[1] == 'h'));
        case 'l': return 'l' << 2 | (1 + (p[1] == 'l'));
        case 'j': case 'z': case 't': case 'L': return *p << 2 | 1;
    }
    return 0;
}

static void store_int_(unsigned length, int sign, void* arg, unsigned long long u)
{
    if (!arg)
        return;
    if (sign) {
        long long v = (long long)u;
        switch (length) {
            case 0:                  *(int*)arg = (int)v; break;
            case 'h' << 2 | 1:       *(short*)arg = (short)v; break;
            case 'h' << 2 | 2:       *(signed char*)arg = (signed char)v; break;
            case 'l' << 2 | 1:       *(long*)arg = (long)v; break;
            case 'l' << 2 | 2:       *(long long*)arg = v; break;
            case 'j' << 2 | 1:       *(intmax_t*)arg = (intmax_t)v; break;
            default:                 *(ptrdiff_t*)arg = (ptrdiff_t)v;
        }
    } else {
        switch (length) {
            case 0:                  *(unsigned*)arg = (unsigned)u; break;
            case 'h' << 2 | 1:       *(unsigned short*)arg = (unsigned short)u; break;
            case 'h' << 2 | 2:       *(unsigned char*)arg = (unsigned char)u; break;
            case 'l' << 2 | 1:       *(unsigned long*)arg = (unsigned long)u; break;
            case 'l' << 2 | 2:       *(unsigned long long*)arg = u; break;
            case 'j' << 2 | 1:       *(uintmax_t*)arg = (uintmax_t)u; break;
            default:                 *(size_t*)arg = (size_t)u;
        }
    }
}

static int scan_int(struct Scanner* sc, int width, int base, int sign, unsigned length, void* arg)
{
    char buf[BUFSZ];
    int n = 0;
    int c;
    int seen_digit = 0;

    skip_ws(sc);
    c = sgetc(sc);
    if (c == EOF)
        return -1;

    if (c == '+' || c == '-') {
        if (n < BUFSZ - 1) buf[n++] = c;
        if (--width == 0) goto done;
        c = sgetc(sc);
    }

    if (c == '0' && width > 0) {
        seen_digit = 1;
        if (n < BUFSZ - 1) buf[n++] = c;
        if (--width == 0) goto done;
        c = sgetc(sc);
        if ((c == 'x' || c == 'X') && (base == 0 || base == 16)) {
            base = 16;
            if (n < BUFSZ - 1) buf[n++] = c;
            seen_digit = 0;
            if (--width == 0) goto done;
            c = sgetc(sc);
        } else if (base == 0) {
            base = 8;
        }
    }

    if (base == 0)
        base = 10;

    while (width > 0 && c != EOF) {
        int d;
        if (c >= '0' && c <= '9')      d = c - '0';
        else if (c >= 'a' && c <= 'z') d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') d = c - 'A' + 10;
        else break;
        if (d >= base) break;

        if (n < BUFSZ - 1) buf[n++] = c;
        seen_digit = 1;
        --width;
        c = sgetc(sc);
    }

done:
    sungetc(sc, c);
    if (!seen_digit)
        return 0;

    buf[n] = '\0';
    char* endp;
    if (sign) {
        long long v = strtoll(buf, &endp, base);
        store_int_(length, 1, arg, (unsigned long long)v);
    } else {
        unsigned long long u = strtoull(buf, &endp, base);
        store_int_(length, 0, arg, u);
    }
    return 1;
}

static int scan_str(struct Scanner* sc, int width, char* arg)
{
    skip_ws(sc);
    int n = 0;
    int c = sgetc(sc);
    if (c == EOF) return -1;

    while (width-- > 0 && c != EOF && !isspace(c)) {
        if (arg) arg[n] = (char)c;
        ++n;
        c = sgetc(sc);
    }
    sungetc(sc, c);
    if (!n) return 0;
    if (arg) arg[n] = '\0';
    return 1;
}

static int scan_chars(struct Scanner* sc, int width, char* arg)
{
    int n = 0;
    int c = sgetc(sc);
    if (c == EOF) return -1;

    while (width-- > 0 && c != EOF) {
        if (arg) arg[n] = (char)c;
        ++n;
        if (width <= 0) break;
        c = sgetc(sc);
    }
    if (!n) return 0;
    return 1;
}

static const char* parse_scanset_(const char* p, unsigned char map[32], int* negate)
{
    *negate = 0;
    if (*p == '^') { *negate = 1; ++p; }

    /* The closing ']', if first, is treated as a literal. */
    int first = 1;
    while (*p && (first || *p != ']')) {
        first = 0;
        unsigned char lo = (unsigned char)*p++;
        if (*p == '-' && p[1] && p[1] != ']') {
            unsigned char hi = (unsigned char)p[1];
            p += 2;
            if (hi < lo) { unsigned char t = lo; lo = hi; hi = t; }
            for (int c = lo; c <= hi; ++c) map[c / 8] |= 1 << (c & 7);
        } else {
            map[lo / 8] |= 1 << (lo & 7);
        }
    }
    if (*p == ']') ++p;
    return p;
}

static int scan_set_(struct Scanner* sc, int width, const unsigned char map[32], int negate, char* arg)
{
    int n = 0;
    int c = sgetc(sc);
    if (c == EOF) return -1;

    while (width-- > 0 && c != EOF) {
        unsigned uc = (unsigned char)c;
        int in_set = (map[uc / 8] >> (uc & 7)) & 1;
        if (in_set == negate) break;
        if (arg) arg[n] = (char)c;
        ++n;
        c = sgetc(sc);
    }
    sungetc(sc, c);
    if (!n) return 0;
    if (arg) arg[n] = '\0';
    return 1;
}

static void store_n_(unsigned length, void* arg, size_t count)
{
    switch (length) {
        case 0:                  *(int*)arg = (int)count; break;
        case 'h' << 2 | 1:       *(short*)arg = (short)count; break;
        case 'h' << 2 | 2:       *(signed char*)arg = (signed char)count; break;
        case 'l' << 2 | 1:       *(long*)arg = (long)count; break;
        case 'l' << 2 | 2:       *(long long*)arg = (long long)count; break;
        case 'j' << 2 | 1:       *(intmax_t*)arg = (intmax_t)count; break;
        default:                 *(ptrdiff_t*)arg = (ptrdiff_t)count;
    }
}

int vfscanf(FILE* restrict stream, const char* restrict format, va_list list)
{
    struct Scanner sc = { stream, 0, -1, 0 };
    int assigned = 0;
    int any_input = 0;

    for (const char* p = format; *p; ) {
        if (isspace((unsigned char)*p)) {
            skip_ws(&sc);
            ++p;
            continue;
        }

        if (*p != '%') {
            int c = sgetc(&sc);
            if (c == EOF)
                return assigned ? assigned : EOF;
            any_input = 1;
            if (c != (unsigned char)*p) {
                sungetc(&sc, c);
                return assigned;
            }
            ++p;
            continue;
        }

        ++p;
        int suppress = 0;
        if (*p == '*') { suppress = 1; ++p; }

        int width = 0;
        while (*p >= '0' && *p <= '9') {
            width = width * 10 + (*p - '0');
            ++p;
        }
        if (!width) width = INT_MAX;

        unsigned length = length_(p);
        p += length & 3;

        int conv = *p++;
        void* arg = suppress ? (void*)0 : va_arg(list, void*);
        int r;

        switch (conv) {
            case 'd': r = scan_int(&sc, width, 10, 1, length, arg); break;
            case 'i': r = scan_int(&sc, width, 0, 1, length, arg); break;
            case 'o': r = scan_int(&sc, width, 8, 0, length, arg); break;
            case 'u': r = scan_int(&sc, width, 10, 0, length, arg); break;
            case 'x':
            case 'X': r = scan_int(&sc, width, 16, 0, length, arg); break;
            case 'p': r = scan_int(&sc, width, 16, 0, 'l' << 2 | 1, arg); break;
            case 's': r = scan_str(&sc, width, arg); break;
            case 'c':
                if (width == INT_MAX) width = 1;
                r = scan_chars(&sc, width, arg);
                break;
            case '[': {
                unsigned char map[32] = {0};
                int negate;
                p = parse_scanset_(p, map, &negate);
                r = scan_set_(&sc, width, map, negate, arg);
                break;
            }
            case 'n':
                if (!suppress)
                    store_n_(length, arg, sc.count);
                r = -2;  /* doesn't count as conversion */
                break;
            case '%': {
                int c = sgetc(&sc);
                if (c == EOF) return assigned ? assigned : EOF;
                any_input = 1;
                if (c != '%') { sungetc(&sc, c); return assigned; }
                r = -2;
                break;
            }
            default:
                return assigned;
        }

        if (r == -1)
            return assigned ? assigned : EOF;
        if (r == 0)
            return assigned;
        if (r == 1) {
            any_input = 1;
            if (!suppress)
                ++assigned;
        }
        /* r == -2: %n / %% — no count, no early return */
        if (r == -2)
            any_input = 1;
    }

    (void)any_input;
    return assigned;
}
