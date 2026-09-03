#ifndef METALLIC_ORACLE_QUAD_H
#define METALLIC_ORACLE_QUAD_H

#include "src/math/long-double/kernel/binary128.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__float128 strtoflt128(const char *, char **);
int quadmath_snprintf(char *, size_t, const char *, ...);

_Static_assert(sizeof(__float128) == 16, "CORE-MATH oracle requires IEEE binary128");

typedef long double quad_unary_fut_(long double);
typedef long double quad_binary_fut_(long double, long double);
typedef __float128 quad_unary_ref_(__float128);
typedef __float128 quad_binary_ref_(__float128, __float128);

static inline __float128 quad_from_bits_(u128 bits)
{
    union {
        __float128 value;
        u128 bits;
    } result = { .bits = bits };
    return result.value;
}

static inline u128 quad_bits_(__float128 value)
{
    union {
        __float128 value;
        u128 bits;
    } result = { .value = value };
    return result.bits;
}

static inline bool quad_nan_bits_(u128 bits)
{
    return (bits & F128_EXP_MASK) == F128_EXP_MASK
        && (bits & F128_MANTISSA_MASK) != 0;
}

static inline bool quad_same_(u128 got, u128 want)
{
    return got == want || (quad_nan_bits_(got) && quad_nan_bits_(want));
}

static bool quad_parse_(const char *token, u128 *bits)
{
    bool negative = token[0] == '-';
    const char *body = token + (token[0] == '+' || negative);
    u128 sign = negative ? F128_SIGN_MASK : 0;

    if (!strcmp(body, "inf")) {
        *bits = sign | F128_EXP_MASK;
        return true;
    }
    if (!strcmp(body, "nan") || !strcmp(body, "qnan")) {
        *bits = sign | F128_EXP_MASK | F128_QUIET_BIT;
        return true;
    }
    if (!strcmp(body, "snan")) {
        *bits = sign | F128_EXP_MASK | 1;
        return true;
    }

    char *end;
    errno = 0;
    __float128 value = strtoflt128(token, &end);
    if (end == token || *end)
        return false;
    *bits = quad_bits_(value);
    return true;
}

static void quad_format_(char buffer[static 64], u128 bits)
{
    if (quadmath_snprintf(buffer, 64, "%+-#.36Qa", quad_from_bits_(bits)) < 0)
        strcpy(buffer, "<format-error>");
}

static char *quad_case_line_(char line[static 256], FILE *stream, size_t number)
{
    if (!strchr(line, '\n') && !feof(stream)) {
        fprintf(stderr, "FAIL: line %zu exceeds 255 bytes\n", number);
        return NULL;
    }

    char *comment = strchr(line, '#');
    if (comment)
        *comment = 0;
    char *start = line;
    while (isspace((unsigned char)*start))
        ++start;
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1]))
        *--end = 0;
    return start;
}

static __attribute__((unused)) int quad_check_unary_(const char *name, quad_unary_fut_ fut,
    quad_unary_ref_ reference, const char *path, size_t expected)
{
    FILE *stream = fopen(path, "r");
    if (!stream) {
        perror(path);
        return 1;
    }

    char line[256];
    size_t line_number = 0;
    size_t count = 0;
    size_t bad = 0;
    while (fgets(line, sizeof line, stream)) {
        char *record = quad_case_line_(line, stream, ++line_number);
        if (!record) {
            fclose(stream);
            return 1;
        }
        /* A few upstream corpora contain literal generator diagnostics. */
        if (!*record || !strcmp(record, "Error 10290") || !strcmp(record, "x***"))
            continue;

        char *tail = record;
        while (*tail && !isspace((unsigned char)*tail))
            ++tail;
        if (*tail) {
            *tail++ = 0;
            while (isspace((unsigned char)*tail))
                ++tail;
        }

        u128 input;
        if (*tail || !quad_parse_(record, &input)) {
            fprintf(stderr, "FAIL: malformed %s corpus line %zu\n", name, line_number);
            fclose(stream);
            return 1;
        }

        u128 got = f128_bits_(fut(f128_from_bits_(input)));
        u128 want = quad_bits_(reference(quad_from_bits_(input)));
        ++count;
        if (!quad_same_(got, want) && bad++ < 20) {
            char got_text[64];
            char want_text[64];
            quad_format_(got_text, got);
            quad_format_(want_text, want);
            fprintf(stderr, "%s line %zu: x=%s, got %s, want %s\n",
                name, line_number, record, got_text, want_text);
        }
    }

    if (ferror(stream)) {
        perror(path);
        fclose(stream);
        return 1;
    }
    fclose(stream);

    if (count != expected) {
        fprintf(stderr, "FAIL: %s read %zu cases, expected %zu\n", name, count, expected);
        return 1;
    }
    if (bad) {
        fprintf(stderr, "FAIL: %s disagreed with CORE-MATH on %zu/%zu cases\n",
            name, bad, count);
        return 1;
    }
    fprintf(stderr, "OK: %s agrees with CORE-MATH on %zu cases\n", name, count);
    return 0;
}

static __attribute__((unused)) int quad_check_binary_(const char *name, quad_binary_fut_ fut,
    quad_binary_ref_ reference, const char *path, size_t expected)
{
    FILE *stream = fopen(path, "r");
    if (!stream) {
        perror(path);
        return 1;
    }

    char line[256];
    size_t line_number = 0;
    size_t count = 0;
    size_t bad = 0;
    while (fgets(line, sizeof line, stream)) {
        char *record = quad_case_line_(line, stream, ++line_number);
        if (!record)
            return fclose(stream), 1;
        if (!*record)
            continue;

        char *second = record;
        while (*second && !isspace((unsigned char)*second))
            ++second;
        if (!*second) {
            fprintf(stderr, "FAIL: malformed %s corpus line %zu\n", name, line_number);
            return fclose(stream), 1;
        }
        *second++ = 0;
        while (isspace((unsigned char)*second))
            ++second;
        char *tail = second;
        while (*tail && !isspace((unsigned char)*tail))
            ++tail;
        if (*tail) {
            *tail++ = 0;
            while (isspace((unsigned char)*tail))
                ++tail;
        }

        u128 first_bits;
        u128 second_bits;
        if (*tail || !quad_parse_(record, &first_bits) || !quad_parse_(second, &second_bits)) {
            fprintf(stderr, "FAIL: malformed %s corpus line %zu\n", name, line_number);
            return fclose(stream), 1;
        }

        u128 got = f128_bits_(fut(f128_from_bits_(first_bits), f128_from_bits_(second_bits)));
        u128 want = quad_bits_(reference(quad_from_bits_(first_bits), quad_from_bits_(second_bits)));
        ++count;
        if (!quad_same_(got, want) && bad++ < 20) {
            char got_text[64];
            char want_text[64];
            quad_format_(got_text, got);
            quad_format_(want_text, want);
            fprintf(stderr, "%s line %zu: first=%s second=%s, got %s, want %s\n",
                name, line_number, record, second, got_text, want_text);
        }
    }

    if (ferror(stream)) {
        perror(path);
        fclose(stream);
        return 1;
    }
    fclose(stream);

    if (count != expected) {
        fprintf(stderr, "FAIL: %s read %zu cases, expected %zu\n", name, count, expected);
        return 1;
    }
    if (bad) {
        fprintf(stderr, "FAIL: %s disagreed with CORE-MATH on %zu/%zu cases\n",
            name, bad, count);
        return 1;
    }
    fprintf(stderr, "OK: %s agrees with CORE-MATH on %zu cases\n", name, count);
    return 0;
}

#endif
