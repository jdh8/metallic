#include "digit.h"
#include "../../math/reinterpret.h"
#include <math.h>

static Scalar nan_(const char s[restrict static 1], char* end[restrict static 1], int tail)
{
    Bitset mantissa = 0;
    unsigned base = 10;
    *end = (char*)(s - 1);

    if (*s == '0') {
        if ((*++s | 32) == 'x') {
            base = 16;
            ++s;
        }
        else base = 8;
    }

    for (unsigned digit = digit_(*s); digit < base; digit = digit_(*++s))
        mantissa = mantissa * base + digit;

    if (*s == tail) {
        *end = (char*)(s + 1);
        const Bitset mask = (Bitset)-1 >> 1;
        const Bitset canonical = reinterpret(Bitset, (Scalar)NAN);
        return reinterpret(Scalar, canonical | (mantissa & mask));
    }

    return NAN;
}
