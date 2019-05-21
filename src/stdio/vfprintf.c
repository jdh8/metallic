#include "FILE.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define FLAG(c) (UINT32_C(1) << ((c) - ' '))

static uint_least32_t _flag(unsigned c)
{
    const uint_least32_t flags = FLAG('-') | FLAG('+') | FLAG(' ') | FLAG('#') | FLAG('0') | FLAG('\'');

    unsigned s = c - ' ';

    return s < 32 ? flags & UINT32_C(1) << s : UINT32_C(0);
}

static int _sign_character(_Bool sign, uint_least32_t flags)
{
    if (sign)
        return '-';

    if (flags & FLAG('+'))
        return '+';

    if (flags & FLAG(' '))
        return ' ';

    return 0;
}

#define TRY(f, ...) do if (f(__VA_ARGS__)) return -1; while (0)

static int _putif(int c, FILE stream[static 1])
{
    return c && stream->_put(c, stream) == EOF;
}

static int _write(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    return stream->_write(buffer, size, stream) != size;
}

static int _pad(int c, size_t length, FILE stream[static 1])
{
    uint64_t vector;

    memset(&vector, c, sizeof(uint64_t));

    for (size_t i = 0; i < length / sizeof(uint64_t); ++i)
        TRY(_write, &vector, sizeof(uint64_t), stream);

    TRY(_write, &vector, length % sizeof(uint64_t), stream);

    return 0;
}

static char* _octal(uintmax_t x, char* s)
{
    for (; x; x >>= 3)
        *--s = (x & 7) + '0';

    return s;
}

static char* _decimal(uintmax_t x, char* s)
{
    for (; x > (unsigned long)-1; x /= 10)
        *--s = x % 10 + '0';

    for (unsigned long y = x; y; y /= 10)
        *--s = y % 10 + '0';

    return s;
}

static char* _hexadecimal(uintmax_t x, char* s, int lower)
{
    for (; x; x >>= 4)
        *--s = "0123456789ABCDEF"[x & 0xF] | lower;

    return s;
}

static int _field(const char* restrict s[restrict static 1], va_list list[restrict static 1])
{
    if (**s == '*') {
        ++*s;
        return va_arg(*list, int);
    }

    const unsigned threshold = (unsigned)-1 / 10;
    unsigned value = 0;
    _Bool overflow = 0;

    for (unsigned digit; (digit = **s - '0') < 10; ++*s) {
        unsigned next = value * 10 + digit;
        overflow |= threshold < value || next < digit;
        value = next;
    }

    if (overflow || INT_MAX < value)
        return INT_MAX;

    return value;
}

static unsigned _length(const char s[static 1])
{
    switch (*s) {
        case 'h':
            return *s << 2 | (1 + (s[1] == 'h'));
        case 'l':
            return *s << 2 | (1 + (s[1] == 'l'));
        case 'j':
        case 'z':
        case 't':
        case 'L':
            return *s << 2 | 1;
    }

    return 0;
}

static intmax_t _pop_signed(unsigned length, va_list list[static 1])
{
    switch (length) {
        case 0:
            return va_arg(*list, int);
        case 'h' << 2 | 1:
            return (short) va_arg(*list, int);
        case 'h' << 2 | 2:
            return (signed char) va_arg(*list, int);
        case 'l' << 2 | 1:
            return va_arg(*list, long);
        case 'l' << 2 | 2:
            return va_arg(*list, long long);
        case 'j' << 2 | 1:
            return va_arg(*list, intmax_t);
    }

    return va_arg(*list, ptrdiff_t);
}

static uintmax_t _pop_unsigned(unsigned length, va_list list[static 1])
{
    switch (length) {
        case 0:
            return va_arg(*list, unsigned);
        case 'h' << 2 | 1:
            return (unsigned short) va_arg(*list, unsigned);
        case 'h' << 2 | 2:
            return (unsigned char) va_arg(*list, unsigned);
        case 'l' << 2 | 1:
            return va_arg(*list, unsigned long);
        case 'l' << 2 | 2:
            return va_arg(*list, unsigned long long);
        case 'j' << 2 | 1:
            return va_arg(*list, uintmax_t);
    }

    return va_arg(*list, size_t);
}

struct Spec
{
    uint_least32_t flags;
    int width;
    int precision;
    unsigned length;
};

#define DECIMAL_DIGITS(T) (((sizeof(T) * CHAR_BIT - ((T)-1 < 0)) * 30103 + 199999) / 100000)

static int _convert_signed(struct Spec spec, FILE stream[static 1], intmax_t arg)
{
    char buffer[DECIMAL_DIGITS(intmax_t)];
    char* end = buffer + sizeof(buffer);
    char* begin = _decimal(arg < 0 ? -arg : arg, end);

    int character = _sign_character(arg < 0, spec.flags);
    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int length = digits + zeros + !!character;
    int padding = spec.width > length ? spec.width - length : 0;

    if (spec.width <= length) {
        TRY(_putif, character, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);

        return length;
    }

    if (spec.flags & FLAG('-')) {
        TRY(_putif, character, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
        TRY(_pad, ' ', padding, stream);
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(_putif, character, stream);
        TRY(_pad, '0', zeros + padding, stream);
        TRY(_write, begin, digits, stream);
    }
    else {
        TRY(_pad, ' ', padding, stream);
        TRY(_putif, character, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
    }

    return spec.width;
}

static int _convert_unsigned(struct Spec spec, FILE stream[static 1], uintmax_t arg)
{
    char buffer[DECIMAL_DIGITS(uintmax_t)];
    char* end = buffer + sizeof(buffer);
    char* begin = _decimal(arg, end);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int length = digits + zeros;
    int padding = spec.width > length ? spec.width - length : 0;

    if (spec.width <= length) {
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);

        return length;
    }

    if (spec.flags & FLAG('-')) {
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
        TRY(_pad, ' ', padding, stream);
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(_pad, '0', zeros + padding, stream);
        TRY(_write, begin, digits, stream);
    }
    else {
        TRY(_pad, ' ', padding, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
    }

    return spec.width;
}

static int _convert_octal(struct Spec spec, FILE stream[static 1], uintmax_t arg)
{
    char buffer[(sizeof(uintmax_t) * CHAR_BIT + 2) / 3];
    char* end = buffer + sizeof(buffer);
    char* begin = _octal(arg, end);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : (spec.flags >> ('#' - ' ')) & 1;
    int length = digits + zeros;
    int padding = spec.width > length ? spec.width - length : 0;

    if (spec.width <= length) {
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);

        return length;
    }

    if (spec.flags & FLAG('-')) {
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
        TRY(_pad, ' ', padding, stream);
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(_pad, '0', zeros + padding, stream);
        TRY(_write, begin, digits, stream);
    }
    else {
        TRY(_pad, ' ', padding, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
    }

    return spec.width;
}

static int _convert_hexadecimal(struct Spec spec, FILE stream[static 1], int format, uintmax_t arg)
{
    const char cache[] = { '0', format };
    char buffer[(sizeof(uintmax_t) * CHAR_BIT + 3) >> 2];
    char* end = buffer + sizeof(buffer);
    char* begin = _hexadecimal(arg, end, format & 0x20);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int prefix = (spec.flags & FLAG('#') && arg) << 1;
    int length = digits + zeros + prefix;
    int padding = spec.width > length ? spec.width - length : 0;

    if (spec.width <= length) {
        TRY(_write, cache, prefix, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);

        return length;
    }

    if (spec.flags & FLAG('-')) {
        TRY(_write, cache, prefix, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
        TRY(_pad, ' ', padding, stream);
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(_write, cache, prefix, stream);
        TRY(_pad, '0', zeros + padding, stream);
        TRY(_write, begin, digits, stream);
    }
    else {
        TRY(_pad, ' ', padding, stream);
        TRY(_write, cache, prefix, stream);
        TRY(_pad, '0', zeros, stream);
        TRY(_write, begin, digits, stream);
    }

    return spec.width;
}

static int _convert(struct Spec spec, FILE stream[static 1], int format, va_list list[static 1])
{
    switch (format) {
        case 'd':
        case 'i':
            return _convert_signed(spec, stream, _pop_signed(spec.length, list));
        case 'o':
            return _convert_octal(spec, stream, _pop_unsigned(spec.length, list));
        case 'u':
            return _convert_unsigned(spec, stream, _pop_unsigned(spec.length, list));
        case 'x':
        case 'X':
            return _convert_hexadecimal(spec, stream, format, _pop_unsigned(spec.length, list));
    }
    return -2;
}

static struct Spec _parse(const char* restrict s[restrict static 1], va_list list[restrict static 1])
{
    uint_least32_t flags = 0;

    for (uint_least32_t bit; (bit = _flag(**s)); ++*s)
        flags |= bit;

    int width = _field(s, list);
    int precision = -1;

    if (width < 0) {
        flags |= FLAG('-');
        width = -width;
    }

    if (**s == '.') {
        ++*s;
        precision = _field(s, list);
    }

    unsigned length = _length(*s);
    struct Spec spec = { flags, width, precision, length };

    *s += length & 3;
    return spec;
}

int vfprintf(FILE stream[restrict static 1], const char format[restrict static 1], va_list list)
{
    size_t count = 0;

    for (const char* s = format; ; ++s) {
        switch (*s) {
            case '\0':
                TRY(_write, format, s - format, stream);
                return s - format + count;

            case '%':
                if (s[1] == '%') {
                    ++s;
                    TRY(_write, format, s - format, stream);
                    count += s - format;
                }
                else {
                    TRY(_write, format, s - format, stream);
                    count += s - format;
                    ++s;

                    struct Spec spec = _parse(&s, &list);
                    int written = _convert(spec, stream, *s, &list);

                    if (written < 0)
                        return written;

                    count += written;
                }
                format = s + 1;
        }
    }
}
