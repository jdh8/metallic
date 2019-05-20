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

static int _putif(int c, FILE stream[restrict static 1])
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

static int _field(const char* restrict s[static 1], va_list list[static 1])
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

static int _convert_signed(struct Spec spec, FILE stream[restrict static 1], intmax_t arg)
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
    else if (spec.precision < 0 && spec.flags & FLAG('0')) {
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

static int _convert_unsigned(struct Spec spec, FILE stream[restrict static 1], uintmax_t arg)
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
    else if (spec.precision < 0 && spec.flags & FLAG('0')) {
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

static int _convert_octal(struct Spec spec, FILE stream[restrict static 1], uintmax_t arg)
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
    else if (spec.precision < 0 && spec.flags & FLAG('0')) {
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

static int _convert(struct Spec spec, FILE stream[restrict static 1], int format, va_list list[restrict static 1])
{
    switch (format) {
        case 'd':
        case 'i':
            return _convert_signed(spec, stream, _pop_signed(spec.length, list));
        case 'o':
            return _convert_octal(spec, stream, _pop_unsigned(spec.length, list));
        case 'u':
            return _convert_unsigned(spec, stream, _pop_unsigned(spec.length, list));
    }
    return -2;
}

static int _print(size_t, FILE[restrict static 1], const char[restrict static 1], va_list);

static int _parse(size_t count, FILE stream[restrict static 1], const char format[restrict static 1], va_list list)
{
    uint_least32_t flags = 0;

    for (uint_least32_t bit; (bit = _flag(*format)); ++format)
        flags |= bit;

    int width = _field(&format, &list);
    int precision = -1;

    if (width < 0) {
        flags |= FLAG('-');
        width = -width;
    }

    if (*format == '.') {
        ++format;
        precision = _field(&format, &list);
    }

    unsigned length = _length(format);
    struct Spec spec = { flags, width, precision, length };
    int written = _convert(spec, stream, *(format += length & 3), &list);

    return written < 0 ? written : _print(count + written, stream, format + 1, list);
}

static int _print(size_t count, FILE stream[restrict static 1], const char format[restrict static 1], va_list list)
{
    for (size_t i = 0; ; ++i) {
        switch (format[i]) {
            case '%':
                if (format[i + 1] == '%') {
                    TRY(_write, format, i + 1, stream);
                    return _print(count + i + 1, stream, format + i + 2, list);
                }
                TRY(_write, format, i, stream);
                return _parse(count + i, stream, format + i + 1, list);

            case '\0':
                TRY(_write, format, i, stream);
                return count + i;
        }
    }
}

int vfprintf(FILE stream[restrict static 1], const char format[restrict static 1], va_list list)
{
    return _print(0, stream, format, list);
}
