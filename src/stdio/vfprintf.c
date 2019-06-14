#include "FILE.h"
#include "putc.h"
#include "../math/reinterpret.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

static size_t strnlen_(const char begin[static 1], size_t length)
{
    const char* end = begin;

    for (; (uintptr_t)end % sizeof(uint64_t) && length--; ++end)
        if (!*end)
            return end - begin;

    const uint64_t* vector = (const uint64_t*)end;
    const uint64_t magic = 0x7EFEFEFEFEFEFEFF;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t)) {
        if (((*vector + magic) ^ ~*vector) & ~magic)
            for (int k = 0; k < sizeof(uint64_t); ++k)
                if (!end[k])
                    return end - begin + k;

        end = (const char*)++vector;
    }

    for (; length--; ++end)
        if (!*end)
            break;

    return end - begin;
}

#define FLAG(c) (UINT32_C(1) << ((c) - ' '))

static uint_least32_t flag_(unsigned c)
{
    const uint_least32_t flags = FLAG('-') | FLAG('+') | FLAG(' ') | FLAG('#') | FLAG('0') | FLAG('\'');

    unsigned s = c - ' ';

    return s < 32 ? flags & UINT32_C(1) << s : UINT32_C(0);
}

static int signchar_(_Bool sign, uint_least32_t flags)
{
    if (sign)
        return '-';

    if (flags & FLAG('+'))
        return '+';

    if (flags & FLAG(' '))
        return ' ';

    return 0;
}

#define TRY(x) do if (x) return -1; while (0)

static int put_(int c, FILE stream[static 1])
{
    return putc_(c, stream) == EOF;
}

static int write_(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    return stream->write(buffer, size, stream) != size;
}

static int pad_(uint8_t c, size_t length, FILE stream[static 1])
{
    uint64_t vector = c * 0x0101010101010101u;

    for (size_t i = 0; i < length / sizeof(uint64_t); ++i)
        TRY(write_(&vector, sizeof(uint64_t), stream));

    TRY(write_(&vector, length % sizeof(uint64_t), stream));

    return 0;
}

static char* octal_(uintmax_t x, char* s)
{
    for (; x; x >>= 3)
        *--s = (x & 7) + '0';

    return s;
}

static char* decimal_(uintmax_t x, char* s)
{
    for (; x > (unsigned long)-1; x /= 10)
        *--s = x % 10 + '0';

    for (unsigned long y = x; y; y /= 10)
        *--s = y % 10 + '0';

    return s;
}

static char* hexadecimal_(uintmax_t x, char* s, int lower)
{
    for (; x; x >>= 4)
        *--s = "0123456789ABCDEF"[x & 0xF] | lower;

    return s;
}

static int field_(const char* restrict s[restrict static 1], va_list list[restrict static 1])
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

static unsigned length_(const char s[static 1])
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

static intmax_t pop_(unsigned length, va_list list[static 1])
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

static uintmax_t popu_(unsigned length, va_list list[static 1])
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

static int converti_(struct Spec spec, FILE stream[static 1], intmax_t arg)
{
    char buffer[DECIMAL_DIGITS(intmax_t)];
    char* end = buffer + sizeof(buffer);
    char* begin = decimal_(arg < 0 ? -arg : arg, end);

    int sign = signchar_(arg < 0, spec.flags);
    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int length = digits + zeros + !!sign;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(sign && put_(sign, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
        TRY(pad_(' ', padding, stream));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(sign && put_(sign, stream));
        TRY(pad_('0', zeros + padding, stream));
        TRY(write_(begin, digits, stream));
    }
    else {
        TRY(pad_(' ', padding, stream));
        TRY(sign && put_(sign, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
    }
    return length + padding;
}

static int convertu_(struct Spec spec, FILE stream[static 1], uintmax_t arg)
{
    char buffer[DECIMAL_DIGITS(uintmax_t)];
    char* end = buffer + sizeof(buffer);
    char* begin = decimal_(arg, end);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int length = digits + zeros;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
        TRY(pad_(' ', padding, stream));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(pad_('0', zeros + padding, stream));
        TRY(write_(begin, digits, stream));
    }
    else {
        TRY(pad_(' ', padding, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
    }
    return length + padding;
}

static int converto_(struct Spec spec, FILE stream[static 1], uintmax_t arg)
{
    char buffer[(sizeof(uintmax_t) * CHAR_BIT + 2) / 3];
    char* end = buffer + sizeof(buffer);
    char* begin = octal_(arg, end);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : (spec.flags >> ('#' - ' ')) & 1;
    int length = digits + zeros;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
        TRY(pad_(' ', padding, stream));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(pad_('0', zeros + padding, stream));
        TRY(write_(begin, digits, stream));
    }
    else {
        TRY(pad_(' ', padding, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
    }
    return length + padding;
}

static int convertx_(struct Spec spec, FILE stream[static 1], int format, uintmax_t arg)
{
    const char cache[] = { '0', format };
    char buffer[(sizeof(uintmax_t) * CHAR_BIT + 3) >> 2];
    char* end = buffer + sizeof(buffer);
    char* begin = hexadecimal_(arg, end, format & 0x20);

    int precision = spec.precision < 0 ? 1 : spec.precision;
    int digits = end - begin;
    int zeros = precision > digits ? precision - digits : 0;
    int prefix = (spec.flags & FLAG('#') && arg) << 1;
    int length = digits + zeros + prefix;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(write_(cache, prefix, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
        TRY(pad_(' ', padding, stream));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(write_(cache, prefix, stream));
        TRY(pad_('0', zeros + padding, stream));
        TRY(write_(begin, digits, stream));
    }
    else {
        TRY(pad_(' ', padding, stream));
        TRY(write_(cache, prefix, stream));
        TRY(pad_('0', zeros, stream));
        TRY(write_(begin, digits, stream));
    }
    return length + padding;
}

static int nonfinite_(struct Spec spec, FILE stream[restrict static 1], int lower, int sign, const char s[restrict static 3])
{
    const char output[] = { s[0] | lower, s[1] | lower, s[2] | lower };
    int length = 3 + !!sign;
    int padding = (spec.width > length) * (spec.width - length);
    _Bool flushleft = spec.flags & FLAG('-');

    if (!flushleft)
        TRY(pad_(' ', padding, stream));

    TRY(sign && put_(sign, stream));
    TRY(write_(output, 3, stream));

    if (flushleft)
        TRY(pad_(' ', padding, stream));

    return length + padding;
}

static int hexfloat_(struct Spec spec, FILE stream[static 1], int format, double arg)
{
    int lower = format & 0x20;
    int sign = signchar_(signbit(arg), spec.flags);

    if (isinf(arg))
        return nonfinite_(spec, stream, lower, sign, "INF");

    if (isnan(arg))
        return nonfinite_(spec, stream, lower, sign, "NAN");

    int64_t magnitude = reinterpret(int64_t, fabs(arg));
    int64_t biased = magnitude >> 52;
    int64_t exp = biased ? biased - 0x3FF : -1022 * !!magnitude;

    char buffer[24] = { '0', 'X' | lower, '0' + !!biased, '.' };
    char* end = buffer + sizeof(buffer);
    char* postfix = decimal_(exp < 0 ? -exp : exp, end);

    if (!exp)
        *--postfix = '0';

    *--postfix = exp < 0 ? '-' : '+';
    *--postfix = 'P' | lower;

    if (spec.precision < 0) {
        uint64_t mantissa = magnitude << 12;
        char* middle = buffer + 4 - !(mantissa || spec.flags & FLAG('#'));

        for (uint64_t i = mantissa; i; i <<= 4)
            *middle++ = "0123456789ABCDEF"[i >> 60] | lower;

        int length = !!sign + (middle - buffer) + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, middle - buffer, stream));
            TRY(write_(postfix, end - postfix, stream));
            TRY(pad_(' ', padding, stream));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 2, stream));
            TRY(pad_('0', padding, stream));
            TRY(write_(buffer + 2, middle - (buffer + 2), stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        else {
            TRY(pad_(' ', padding, stream));
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, middle - buffer, stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        return length + padding;
    }
    else if (spec.precision < 13) {
        uint64_t significand = (magnitude & 0x000FFFFFFFFFFFFF) | (uint64_t)!!biased << 52;
        uint64_t low = significand << (12 + spec.precision * 4);
        uint64_t high = significand >> (52 - spec.precision * 4);
        uint64_t carry = ((high & 1) | low) > 0x8000000000000000;
        uint64_t rounded = high + carry;

        for (; rounded > 0xF; rounded >>= 4)
            *--postfix = "0123456789ABCDEF"[rounded & 0xF] | lower;

        if (spec.precision || spec.flags & FLAG('#'))
            *--postfix = '.';

        *--postfix = "0123456789ABCDEF"[rounded & 0xF] | lower;

        int length = !!sign + 2 + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 2, stream));
            TRY(write_(postfix, end - postfix, stream));
            TRY(pad_(' ', padding, stream));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 2, stream));
            TRY(pad_('0', padding, stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        else {
            TRY(pad_(' ', padding, stream));
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 2, stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        return length + padding;
    }
    else {
        for (int i = 16; i > 3; --i) {
            buffer[i] = "0123456789ABCDEF"[magnitude & 0xF] | lower;
            magnitude >>= 4;
        }

        int zeros = spec.precision - 13;
        int length = !!sign + 17 + zeros + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 17, stream));
            TRY(pad_('0', zeros, stream));
            TRY(write_(postfix, end - postfix, stream));
            TRY(pad_(' ', padding, stream));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 2, stream));
            TRY(pad_('0', padding, stream));
            TRY(write_(buffer + 2, 15, stream));
            TRY(pad_('0', zeros, stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        else {
            TRY(pad_(' ', padding, stream));
            TRY(sign && put_(sign, stream));
            TRY(write_(buffer, 17, stream));
            TRY(pad_('0', zeros, stream));
            TRY(write_(postfix, end - postfix, stream));
        }
        return length + padding;
    }
}

static int converta_(struct Spec spec, FILE stream[static 1], int format, va_list list[static 1])
{
    if (spec.length == ('L' << 2 | 1)) {
        switch (LDBL_MANT_DIG) {
            case 53:
                return hexfloat_(spec, stream, format, va_arg(*list, long double));
            default:
                return -2;
        }
    }
    return hexfloat_(spec, stream, format, va_arg(*list, double));
}

static int convertc_(struct Spec spec, FILE stream[static 1], va_list list[static 1])
{
    if (spec.length >> 2 == 'l') {
        mbstate_t state = {};
        char buffer[MB_LEN_MAX];
        size_t length = wcrtomb(buffer, va_arg(*list, wint_t), &state);

        if (length == -1)
            return -3;

        TRY(write_(buffer, length, stream));
        return length;
    }

    TRY(put_(va_arg(*list, int), stream));
    return 1;
}

static int converts_(struct Spec spec, FILE stream[static 1], va_list list[static 1])
{
    size_t precision = spec.precision < 0 ? -1 : spec.precision;

    if (spec.length >> 2 == 'l') {
        mbstate_t state = {};
        size_t count = 0;

        for (const wchar_t* s = va_arg(*list, wchar_t*); *s; ++s) {
            char buffer[MB_LEN_MAX];
            size_t length = wcrtomb(buffer, *s, &state);

            if (length == -1)
                return -3;

            if (precision < count + length)
                break;

            TRY(write_(buffer, length, stream));
            count += length;
        }

        return count;
    }

    const char* s = va_arg(*list, char*);
    size_t length = strnlen_(s, precision);

    TRY(write_(s, length, stream));

    return length;
}

static int convertp_(FILE stream[static 1], void* arg)
{
    if (!arg) {
        TRY(write_("NULL", 4, stream));
        return 4;
    }

    char buffer[(sizeof(uintptr_t) * CHAR_BIT + 3) >> 2];
    char* end = buffer + sizeof(buffer);
    char* begin = hexadecimal_((uintptr_t)arg, end, 0);
    ptrdiff_t length = end - begin;

    TRY(write_("0x", 2, stream));
    TRY(write_(begin, length, stream));

    return length + 2;
}

static int storen_(struct Spec spec, size_t count, FILE stream[static 1], va_list list[static 1])
{
    switch (spec.length) {
        case 0:
            *va_arg(*list, int*) = count;
            break;
        case 'h' << 2 | 1:
            *va_arg(*list, short*) = count;
            break;
        case 'h' << 2 | 2:
            *va_arg(*list, signed char*) = count;
            break;
        case 'l' << 2 | 1:
            *va_arg(*list, long*) = count;
            break;
        case 'l' << 2 | 2:
            *va_arg(*list, long long*) = count;
            break;
        case 'j' << 2 | 1:
            *va_arg(*list, intmax_t*) = count;
            break;
        default:
            *va_arg(*list, ptrdiff_t*) = count;
    }

    return 0;
}

static int convert_(struct Spec spec, size_t count, FILE stream[static 1], int format, va_list list[static 1])
{
    switch (format) {
        case 'd':
        case 'i':
            return converti_(spec, stream, pop_(spec.length, list));
        case 'o':
            return converto_(spec, stream, popu_(spec.length, list));
        case 'u':
            return convertu_(spec, stream, popu_(spec.length, list));
        case 'x':
        case 'X':
            return convertx_(spec, stream, format, popu_(spec.length, list));
        case 'a':
        case 'A':
            return converta_(spec, stream, format, list);
        case 'c':
            return convertc_(spec, stream, list);
        case 's':
            return converts_(spec, stream, list);
        case 'p':
            return convertp_(stream, va_arg(*list, void*));
        case 'n':
            return storen_(spec, count, stream, list);
    }
    return -2;
}

static struct Spec parse_(const char* restrict s[restrict static 1], va_list list[restrict static 1])
{
    uint_least32_t flags = 0;

    for (uint_least32_t bit; (bit = flag_(**s)); ++*s)
        flags |= bit;

    int width = field_(s, list);
    int precision = -1;

    if (width < 0) {
        flags |= FLAG('-');
        width = -width;
    }

    if (**s == '.') {
        ++*s;
        precision = field_(s, list);
    }

    unsigned length = length_(*s);
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
                TRY(write_(format, s - format, stream));
                return s - format + count;

            case '%':
                if (s[1] == '%') {
                    ++s;
                    TRY(write_(format, s - format, stream));
                    count += s - format;
                }
                else {
                    TRY(write_(format, s - format, stream));
                    count += s - format;
                    ++s;

                    struct Spec spec = parse_(&s, &list);
                    int written = convert_(spec, count, stream, *s, &list);

                    if (written < 0)
                        return written;

                    count += written;
                }
                format = s + 1;
        }
    }
}
