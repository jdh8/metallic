#include "FILE.h"
#include "putc.h"
#include "../math/reinterpret.h"
#include "../string/aliased.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#define DECIMAL_DIGITS(T) (((sizeof(T) * CHAR_BIT - (((T)-1 < 0)) * 30103 + 199999) / 100000))
#define FLAG(c) (UINT32_C(1) << ((c) - ' '))
#define TRY(x) do if (x) return -1; while (0)

static size_t strnlen_(const char begin[static 1], size_t length)
{
    const char* end = begin;

    for (; (uintptr_t)end % sizeof(uint64_t) && length--; ++end)
        if (!*end)
            return end - begin;

    const uint64_t ALIASED* vector = (const uint64_t*)end;
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

static int put_(FILE stream[static 1], int c)
{
    return putc_(stream, c) == EOF;
}

static int write_(FILE stream[restrict static 1], const void* restrict buffer, size_t size)
{
    return stream->write(stream, buffer, size) != size;
}

static int pad_(FILE stream[static 1], uint8_t c, size_t length)
{
    uint64_t vector = c * 0x0101010101010101u;

    for (size_t i = 0; i < length / sizeof(uint64_t); ++i)
        TRY(write_(stream, &vector, sizeof(uint64_t)));

    TRY(write_(stream, &vector, length % sizeof(uint64_t)));

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

static intmax_t pop_signed_(unsigned length, va_list list[static 1])
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

static uintmax_t pop_unsigned_(unsigned length, va_list list[static 1])
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

static int convert_i_(FILE stream[static 1], struct Spec spec, intmax_t arg)
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
        TRY(sign && put_(stream, sign));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(sign && put_(stream, sign));
        TRY(pad_(stream, '0', zeros + padding));
        TRY(write_(stream, begin, digits));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(sign && put_(stream, sign));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
    }
    return length + padding;
}

static int convert_u_(FILE stream[static 1], struct Spec spec, uintmax_t arg)
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
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(pad_(stream, '0', zeros + padding));
        TRY(write_(stream, begin, digits));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
    }
    return length + padding;
}

static int convert_o_(FILE stream[static 1], struct Spec spec, uintmax_t arg)
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
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(pad_(stream, '0', zeros + padding));
        TRY(write_(stream, begin, digits));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
    }
    return length + padding;
}

static int convert_x_(FILE stream[static 1], struct Spec spec, int format, uintmax_t arg)
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
        TRY(write_(stream, cache, prefix));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0') && spec.precision < 0) {
        TRY(write_(stream, cache, prefix));
        TRY(pad_(stream, '0', zeros + padding));
        TRY(write_(stream, begin, digits));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(write_(stream, cache, prefix));
        TRY(pad_(stream, '0', zeros));
        TRY(write_(stream, begin, digits));
    }
    return length + padding;
}

static int nonfinite_(FILE stream[restrict static 1], struct Spec spec, int lower, int sign, const char s[restrict static 3])
{
    const char output[] = { s[0] | lower, s[1] | lower, s[2] | lower };
    int length = 3 + !!sign;
    int padding = (spec.width > length) * (spec.width - length);
    _Bool flushleft = spec.flags & FLAG('-');

    if (!flushleft)
        TRY(pad_(stream, ' ', padding));

    TRY(sign && put_(stream, sign));
    TRY(write_(stream, output, 3));

    if (flushleft)
        TRY(pad_(stream, ' ', padding));

    return length + padding;
}

static int64_t ifrexp_(int64_t magnitude, int exp[static 1])
{
    int64_t significand = (magnitude & 0x000FFFFFFFFFFFFF) | 0x0010000000000000;
    int shift = __builtin_ctzll(significand);
    *exp = (int)(magnitude >> 52) + shift - 1075;
    return significand >> shift;
}

static size_t limbs_set_u64_(uint64_t i, uint_least32_t decimal[static 3])
{
    uint64_t q = i / 1000000000u;

    decimal[0] = i % 1000000000u;
    decimal[1] = q % 1000000000u;
    decimal[2] = q / 1000000000u;

    return decimal[2] ? 3 : decimal[1] ? 2 : !!decimal[0];
}

static size_t limbs_set_u18d_(uint64_t i, uint_least32_t decimal[static 2])
{
    decimal[0] = i % 1000000000u;
    decimal[1] = i / 1000000000u;

    return decimal[1] ? 2 : !!decimal[0];
}

static size_t limbs_mul_(uint_least32_t* restrict product,
    const uint_least32_t* restrict x, size_t xn,
    const uint_least32_t* restrict y, size_t yn)
{
    for (size_t k = 0; k < xn + yn; ++k)
        product[k] = 0;

    for (size_t i = 0; i < xn; ++i) {
        for (size_t j = 0; j < yn; ++j) {
            size_t k = i + j;
            uint64_t z = (uint64_t)x[i] * y[j] + product[k];
            product[k] = z % 1000000000u;
            product[k + 1] += z / 1000000000u;
        }
    }
    return xn + yn - !product[xn + yn - 1];
}

static size_t bits_to_limbs_(size_t bits)
{
    return (bits * 30103 + 899999) / 900000;
}

static size_t limbs_ldexp_(uint_least32_t* restrict product, const uint_least32_t* restrict v, size_t n, size_t shift)
{
    uint_least32_t even[3];
    size_t xn = limbs_mul_(product, v, n, even, limbs_set_u64_((uint64_t)1 << (shift & 63), even));

    uint_least32_t y[bits_to_limbs_(shift & -64) + 2];
    size_t yn = 2;

    y[0] = 294967296;
    y[1] = 4;

    for (size_t power = shift >> 6; power; power >>= 1) {
        uint_least32_t x[yn];

        for (size_t i = 0; i < yn; ++i)
            x[i] = y[i];

        yn = limbs_mul_(y, x, yn, x, yn);

        if (power & 1) {
            uint_least32_t x[xn];

            for (size_t i = 0; i < xn; ++i)
                x[i] = product[i];

            xn = limbs_mul_(product, x, xn, y, yn);
        }
    }
    return xn;
}

static uint64_t exp5u64_(size_t power)
{
    return (power & 1 ? (uint64_t)1e01 >>  1 : 1)
        * (power &  2 ? (uint64_t)1e02 >>  2 : 1)
        * (power &  4 ? (uint64_t)1e04 >>  4 : 1)
        * (power &  8 ? (uint64_t)1e08 >>  8 : 1)
        * (power & 16 ? (uint64_t)1e16 >> 16 : 1);
}

static size_t fives_to_limbs_(size_t bits)
{
    return (bits * 699 + 8999) / 9000;
}

static size_t limbs_scal5n_(uint_least32_t* restrict product, const uint_least32_t* restrict v, size_t n, size_t power)
{
    uint_least32_t even[3];
    size_t xn = limbs_mul_(product, v, n, even, limbs_set_u64_(exp5u64_(power % 28), even));

    uint_least32_t y[fives_to_limbs_(power)];
    size_t yn = 2;

    y[0] = 103515625;
    y[1] = 6;

    for (power /= 28; power; power >>= 1) {
        uint_least32_t x[yn];

        for (size_t i = 0; i < yn; ++i)
            x[i] = y[i];

        yn = limbs_mul_(y, x, yn, x, yn);

        if (power & 1) {
            uint_least32_t x[xn];

            for (size_t i = 0; i < xn; ++i)
                x[i] = product[i];

            xn = limbs_mul_(product, x, xn, y, yn);
        }
    }
    return xn;
}

static double limbs_getfrac_(const uint_least32_t* x, ptrdiff_t index)
{
    if (index > 0) {
        uint_least32_t highest = x[index - 1];

        if (highest < 500000000)
            return 0;

        if (highest == 500000000) {
            for (ptrdiff_t i = index - 2; i >= 0; --i)
                if (x[i])
                    return 1;
            return 0.5;
        }
        return 1;
    }
    return 0;
}

static uint_fast32_t limb_placed_rint_(double x, unsigned place)
{
    uint_fast32_t factor = (place & 1 ? 10 : 1)
        * (place & 2 ? (uint_fast32_t)1e2 : 1)
        * (place & 4 ? (uint_fast32_t)1e4 : 1)
        * (place & 8 ? (uint_fast32_t)1e8 : 1);

    return (uint_fast32_t)rint(x / factor) * factor;
}

static size_t limbs_placed_rint_(uint_least32_t* x, size_t xn, int place)
{
    if (place <= 0)
        return xn;

    if (place <= 9 * xn) {
        unsigned index = place / 9u;
        unsigned shift = place % 9u;

        x[index] = limb_placed_rint_(x[index] + limbs_getfrac_(x, index), shift);

        for (; index < xn && x[index] >= 1000000000; ++index) {
            x[index] -= 1000000000;
            x[index + 1]++;
        }
        return xn + (index == xn);
    }

    return 0;
}

static uint64_t limbs_modf_(double x, int prec, char* buffer)
{
    if (!prec)
        return rint(x);

    uint64_t i = x;
    uint64_t frac = 0x1p60 * (x - i);

    for (int k = 0; k < prec - 1; ++k) {
        frac *= 10;
        buffer[k] = '0' + (unsigned)(frac >> 60);
        frac &= 0x0FFFFFFFFFFFFFFF;
    }

    buffer[prec - 1] = '0' + (unsigned) rint(frac * 0xap-60);

    for (int k = prec - 1; k > 0; --k) {
        if (buffer[k] > '9') {
            buffer[k] -= 10;
            ++buffer[k - 1];
        }
    }

    if (*buffer > '9') {
        *buffer -= 10;
        return i + 1;
    }

    return i;
}

static int limbs_write_(FILE stream[static 1], const uint_least32_t* x, size_t length)
{
    char buffer[9];

    while (length--) {
        uint_least32_t piece = x[length];

        for (int j = 8; j >= 0; --j) {
            buffer[j] = piece % 10 + '0';
            piece /= 10;
        }
        TRY(write_(stream, buffer, 9));
    }
    return 0;
}

static int common_fixed_small_(FILE stream[static 1], struct Spec spec, int sign, const uint_least32_t* base, size_t size, int exp)
{
    _Bool pointed = spec.precision || spec.flags & FLAG('#');
    int length = !!sign + 1 + pointed + spec.precision;
    int padding = (spec.width > length) * (spec.width - length);

    uint_least32_t big[fives_to_limbs_(-exp) + 2];
    unsigned place = -exp - spec.precision;
    size_t end = limbs_placed_rint_(big, limbs_scal5n_(big, base, size, -exp), place);
    size_t begin = place / 9 * !!end;

    fprintf(stderr, "[DEBUG] precision: %i\n", spec.precision);
    fprintf(stderr, "[DEBUG] exp: %i\n", exp);
    fprintf(stderr, "[DEBUG] capacity: %zu\n", sizeof(big) / sizeof(uint_least32_t));
    fprintf(stderr, "[DEBUG] begin: %zu\n", begin);
    fprintf(stderr, "[DEBUG] end: %zu\n", end);

    TRY(limbs_write_(stream, big + begin, end - begin));

    return length + padding;
}

static int fixed_small_(FILE stream[static 1], struct Spec spec, int sign, double magnitude)
{
    int64_t bits = reinterpret(int64_t, magnitude);
    uint_least32_t odd[2];
    int shift;
    size_t size = limbs_set_u18d_(ifrexp_(bits, &shift), odd);

    return common_fixed_small_(stream, spec, sign, odd, size, shift);
}

static int fixed_moderate_(FILE stream[static 1], struct Spec spec, int sign, double magnitude)
{
    _Bool pointed = spec.precision || spec.flags & FLAG('#');
    char frac[spec.precision];
    uint64_t truncated = limbs_modf_(magnitude, spec.precision, frac);

    char integ[DECIMAL_DIGITS(uint64_t)];
    char* end = integ + sizeof(integ);
    char* begin = decimal_(truncated, end);

    if (!truncated)
        *--begin = '0';

    int length = !!sign + (end - begin) + pointed + spec.precision;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(sign && put_(stream, sign));
        TRY(write_(stream, begin, end - begin));
        TRY(pointed && put_(stream, '.'));
        TRY(write_(stream, frac, spec.precision));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0')) {
        TRY(sign && put_(stream, sign));
        TRY(pad_(stream, '0', padding));
        TRY(write_(stream, begin, end - begin));
        TRY(pointed && put_(stream, '.'));
        TRY(write_(stream, frac, spec.precision));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(sign && put_(stream, sign));
        TRY(write_(stream, begin, end - begin));
        TRY(pointed && put_(stream, '.'));
        TRY(write_(stream, frac, spec.precision));
    }
    return length + padding;
}

static int fixed_bigint_(FILE stream[static 1], struct Spec spec, int sign, double magnitude)
{
    _Bool pointed = spec.precision || spec.flags & FLAG('#');
    int64_t bits = reinterpret(int64_t, magnitude);

    uint_least32_t big[bits_to_limbs_((bits >> 52) - 1022) + 1];
    uint_least32_t odd[2];
    int shift;
    size_t size = limbs_set_u18d_(ifrexp_(bits, &shift), odd);
    size_t limbs = limbs_ldexp_(big, odd, size, shift);

    char buffer[9];
    char* end = buffer + 9;
    char* begin = decimal_(big[limbs - 1], end);

    int length = !!sign + (end - begin) + 9 * (limbs - 1) + pointed + spec.precision;
    int padding = (spec.width > length) * (spec.width - length);

    if (spec.flags & FLAG('-')) {
        TRY(sign && put_(stream, sign));
        TRY(write_(stream, begin, end - begin));
        TRY(limbs_write_(stream, big, limbs - 1));
        TRY(pointed && put_(stream, '.'));
        TRY(pad_(stream, '0', spec.precision));
        TRY(pad_(stream, ' ', padding));
    }
    else if (spec.flags & FLAG('0')) {
        TRY(sign && put_(stream, sign));
        TRY(pad_(stream, '0', padding));
        TRY(write_(stream, begin, end - begin));
        TRY(limbs_write_(stream, big, limbs - 1));
        TRY(pointed && put_(stream, '.'));
        TRY(pad_(stream, '0', spec.precision));
    }
    else {
        TRY(pad_(stream, ' ', padding));
        TRY(sign && put_(stream, sign));
        TRY(write_(stream, begin, end - begin));
        TRY(limbs_write_(stream, big, limbs - 1));
        TRY(pointed && put_(stream, '.'));
        TRY(pad_(stream, '0', spec.precision));
    }
    return length + padding;
}

static int fixed_(FILE stream[static 1], struct Spec spec, int lower, double arg)
{
    const int64_t inf = 0x7FF0000000000000;
    int sign = signchar_(signbit(arg), spec.flags);
    double magnitude = fabs(arg);
    int64_t bits = reinterpret(int64_t, magnitude);

    if (bits < 0x3F70000000000000)
        return fixed_small_(stream, spec, sign, magnitude);

    if (bits < 0x43F0000000000000)
        return fixed_moderate_(stream, spec, sign, magnitude);

    if (bits < inf)
        return fixed_bigint_(stream, spec, sign, magnitude);

    return nonfinite_(stream, spec, lower, sign, bits == inf ? "INF" : "NAN");
}

static int convert_f_(FILE stream[static 1], struct Spec spec, int lower, va_list list[static 1])
{
    if (spec.length == ('L' << 2 | 1)) {
        switch (LDBL_MANT_DIG) {
            case 53:
                return fixed_(stream, spec, lower, va_arg(*list, long double));
            default:
                return -2;
        }
    }
    return fixed_(stream, spec, lower, va_arg(*list, double));
}

static int hexfloat_(FILE stream[static 1], struct Spec spec, int lower, double arg)
{
    int sign = signchar_(signbit(arg), spec.flags);

    if (isinf(arg))
        return nonfinite_(stream, spec, lower, sign, "INF");

    if (isnan(arg))
        return nonfinite_(stream, spec, lower, sign, "NAN");

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
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, middle - buffer));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, buffer + 2, middle - (buffer + 2)));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, middle - buffer));
            TRY(write_(stream, postfix, end - postfix));
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
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(write_(stream, postfix, end - postfix));
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
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 17));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, buffer + 2, 15));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 17));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
        }
        return length + padding;
    }
}

static int hexfloatq_(FILE stream[static 1], struct Spec spec, int lower, long double arg)
{
    int sign = signchar_(signbit(arg), spec.flags);

    if (isinf(arg))
        return nonfinite_(stream, spec, lower, sign, "INF");

    if (isnan(arg))
        return nonfinite_(stream, spec, lower, sign, "NAN");

#ifdef __SIZEOF_INT128__
    unsigned __int128 magnitude = reinterpret(unsigned __int128, fabsl(arg));
    int64_t biased = magnitude >> 112;
    int64_t exp = biased ? biased - 0x3FFF : -16382 * !!magnitude;

    char buffer[40] = { '0', 'X' | lower, '0' + !!biased, '.' };
    char* end = buffer + sizeof(buffer);
    char* postfix = decimal_(exp < 0 ? -exp : exp, end);

    if (!exp)
        *--postfix = '0';

    *--postfix = exp < 0 ? '-' : '+';
    *--postfix = 'P' | lower;

    if (spec.precision < 0) {
        unsigned __int128 mantissa = magnitude << 16;
        char* middle = buffer + 4 - !(mantissa || spec.flags & FLAG('#'));

        for (unsigned __int128 i = mantissa; i; i <<= 4)
            *middle++ = "0123456789ABCDEF"[i >> 124] | lower;

        int length = !!sign + (middle - buffer) + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, middle - buffer));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, buffer + 2, middle - (buffer + 2)));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, middle - buffer));
            TRY(write_(stream, postfix, end - postfix));
        }
        return length + padding;
    }
    else if (spec.precision < 28) {
        const unsigned __int128 mask = (unsigned __int128)-1 >> 16;
        const unsigned __int128 threshold = (unsigned __int128)1 << 127;

        unsigned __int128 significand = (magnitude & mask) | (unsigned __int128)!!biased << 112;
        unsigned __int128 low = significand << (16 + spec.precision * 4);
        unsigned __int128 high = significand >> (112 - spec.precision * 4);
        unsigned __int128 carry = ((high & 1) | low) > threshold;
        unsigned __int128 rounded = high + carry;

        for (; rounded > 0xF; rounded >>= 4)
            *--postfix = "0123456789ABCDEF"[rounded & 0xF] | lower;

        if (spec.precision || spec.flags & FLAG('#'))
            *--postfix = '.';

        *--postfix = "0123456789ABCDEF"[rounded & 0xF] | lower;

        int length = !!sign + 2 + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(write_(stream, postfix, end - postfix));
        }
        return length + padding;
    }
    else {
        for (int i = 31; i > 3; --i) {
            buffer[i] = "0123456789ABCDEF"[magnitude & 0xF] | lower;
            magnitude >>= 4;
        }

        int zeros = spec.precision - 28;
        int length = !!sign + 32 + zeros + (end - postfix);
        int padding = (spec.width > length) * (spec.width - length);

        if (spec.flags & FLAG('-')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 32));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
            TRY(pad_(stream, ' ', padding));
        }
        else if (spec.flags & FLAG('0')) {
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 2));
            TRY(pad_(stream, '0', padding));
            TRY(write_(stream, buffer + 2, 30));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
        }
        else {
            TRY(pad_(stream, ' ', padding));
            TRY(sign && put_(stream, sign));
            TRY(write_(stream, buffer, 32));
            TRY(pad_(stream, '0', zeros));
            TRY(write_(stream, postfix, end - postfix));
        }
        return length + padding;
    }
#else
    return -2;
#endif
}

static int convert_a_(FILE stream[static 1], struct Spec spec, int lower, va_list list[static 1])
{
    if (spec.length == ('L' << 2 | 1)) {
        switch (LDBL_MANT_DIG) {
            case 53:
                return hexfloat_(stream, spec, lower, va_arg(*list, long double));
            case 113:
                return hexfloatq_(stream, spec, lower, va_arg(*list, long double));
            default:
                return -2;
        }
    }
    return hexfloat_(stream, spec, lower, va_arg(*list, double));
}

static int convert_c_(FILE stream[static 1], struct Spec spec, va_list list[static 1])
{
    if (spec.length >> 2 == 'l') {
        mbstate_t state = { 0 };
        char buffer[MB_LEN_MAX];
        size_t length = wcrtomb(buffer, va_arg(*list, wint_t), &state);

        if (length == -1)
            return -3;

        TRY(write_(stream, buffer, length));
        return length;
    }

    TRY(put_(stream, va_arg(*list, int)));
    return 1;
}

static int convert_s_(FILE stream[static 1], struct Spec spec, va_list list[static 1])
{
    size_t precision = spec.precision < 0 ? -1 : spec.precision;

    if (spec.length >> 2 == 'l') {
        mbstate_t state = { 0 };
        size_t count = 0;

        for (const wchar_t* s = va_arg(*list, wchar_t*); *s; ++s) {
            char buffer[MB_LEN_MAX];
            size_t length = wcrtomb(buffer, *s, &state);

            if (length == -1)
                return -3;

            if (precision < count + length)
                break;

            TRY(write_(stream, buffer, length));
            count += length;
        }

        return count;
    }

    const char* s = va_arg(*list, char*);
    size_t length = strnlen_(s, precision);

    TRY(write_(stream, s, length));

    return length;
}

static int convert_p_(FILE stream[static 1], void* arg)
{
    if (!arg) {
        TRY(write_(stream, "NULL", 4));
        return 4;
    }

    char buffer[(sizeof(uintptr_t) * CHAR_BIT + 3) >> 2];
    char* end = buffer + sizeof(buffer);
    char* begin = hexadecimal_((uintptr_t)arg, end, 0);
    ptrdiff_t length = end - begin;

    TRY(write_(stream, "0x", 2));
    TRY(write_(stream, begin, length));

    return length + 2;
}

static int store_n_(FILE stream[static 1], struct Spec spec, size_t count, va_list list[static 1])
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

static struct Spec floatspec_(struct Spec spec)
{
    if (spec.precision < 0)
        spec.precision = 6;

    return spec;
}

static int convert_(FILE stream[static 1], struct Spec spec, size_t count, int format, va_list list[static 1])
{
    int lower = format & 0x20;

    switch (format) {
        case 'd':
        case 'i':
            return convert_i_(stream, spec, pop_signed_(spec.length, list));
        case 'o':
            return convert_o_(stream, spec, pop_unsigned_(spec.length, list));
        case 'u':
            return convert_u_(stream, spec, pop_unsigned_(spec.length, list));
        case 'x':
        case 'X':
            return convert_x_(stream, spec, format, pop_unsigned_(spec.length, list));
        case 'f':
        case 'F':
            return convert_f_(stream, floatspec_(spec), lower, list);
        case 'a':
        case 'A':
            return convert_a_(stream, spec, lower, list);
        case 'c':
            return convert_c_(stream, spec, list);
        case 's':
            return convert_s_(stream, spec, list);
        case 'p':
            return convert_p_(stream, va_arg(*list, void*));
        case 'n':
            return store_n_(stream, spec, count, list);
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
                TRY(write_(stream, format, s - format));
                return s - format + count;

            case '%':
                if (s[1] == '%') {
                    ++s;
                    TRY(write_(stream, format, s - format));
                    count += s - format;
                }
                else {
                    TRY(write_(stream, format, s - format));
                    count += s - format;
                    ++s;

                    struct Spec spec = parse_(&s, &list);
                    int written = convert_(stream, spec, count, *s, &list);

                    if (written < 0)
                        return written;

                    count += written;
                }
                format = s + 1;
        }
    }
}
