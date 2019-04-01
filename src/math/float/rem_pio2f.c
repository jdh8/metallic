#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Get 96 bits of 2/π with `offset` bits skipped */
static uint64_t _segment(int offset, uint32_t low[static 1])
{
    const uint64_t bits[] = { 0xA2F9836E4E441529, 0xFC2757D1F534DDC0, 0xDB6295993C439041, 0xFE5163ABDEBBC561 };

    int index = offset >> 6;
    int shift = offset & 63;

    *low = shift > 32 ? bits[index + 2] >> (96 - shift) | bits[index + 1] << (shift - 32) : bits[index + 1] >> (32 - shift);
    return shift ? bits[index + 1] >> (64 - shift) | bits[index] << shift : bits[index];
}

/* Argument reduction for trigonometric functions
 *
 * The prototype of this function resembles `__rem_pio2` in GCC, but this
 * function is only for `float`.  Pseudocode is as follows.
 *
 * quotient = nearest integer of x / (π/2)
 * *y = IEEE remainder of x / (π/2) = x - quotient * (π/2)
 *
 * return quotient with accurate sign and last 2 bits
 */
int __rem_pio2f(float x, double y[static 1])
{
    const double pi_2[] = { 0x1.921fb5p0, 0x1.110b4611a6263p-26 };
    const double _2_pi = 0.63661977236758134308;

    int32_t i = reinterpret(int32_t, x);
    int32_t magnitude = i & 0x7FFFFFFF;

    if (magnitude < 0x4DC90FDB) { /* π * 0x1p27 */
        double q = rint(_2_pi * x) + 0;
        *y = x - q * pi_2[0] - q * pi_2[1];
        return q;
    }

    if (magnitude >= 0x7F800000) {
        *y = x - x;
        return 0;
    }

    uint32_t low;
    uint64_t high = _segment((magnitude >> 23) - 152, &low);
    uint64_t significand = (i & 0x007FFFFF) | 0x00800000;

    /* First 64 bits of fractional part of x/(2π) */
    uint64_t product = significand * high + ((significand * low) >> 32);

    int64_t r = product << 2;
    int q = (product >> 62) + (r < 0);

    *y = copysign(8.51530395021638647334e-20 /* π * 0x1p-65 */, x) * r;

    return i < 0 ? -q : q;
}
