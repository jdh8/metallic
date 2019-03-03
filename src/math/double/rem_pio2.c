#include "../../soft/integer/kernel/umuldi.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Argument reduction for |x| < π * 0x1p27 */
static int _small(double x, double y[static 2])
{
    const double pi_2[] = { 0x1.921FB5p0, 0x1.110B46p-26, 0x1.1A62633145C07p-54 };
    const double _2_pi = 0.63661977236758134308;

    double q = rint(_2_pi * x) + 0;
    double a = x - q * pi_2[0];
    double b = q * -pi_2[1];
    double s = a + b;
    double e = a - s + b - q * pi_2[2];

    y[0] = s + e;
    y[1] = s - y[0] + e;

    return q;
}

/* Get 192 bits of 2/π with `offset` bits skipped */
static void _segment(uint64_t segment[static 3], int offset)
{
    const uint64_t bits[] = {
        0xA2F9836E4E441529, 0xFC2757D1F534DDC0, 0xDB6295993C439041, 0xFE5163ABDEBBC561,
        0xB7246E3A424DD2E0, 0x06492EEA09D1921C, 0xFE1DEB1CB129A73E, 0xE88235F52EBB4484,
        0xE99C7026B45F7E41, 0x3991D639835339F4, 0x9C845F8BBDF9283B, 0x1FF897FFDE05980F,
        0xEF2F118B5A0A6D1F, 0x6D367ECF27CB09B7, 0x4F463F669E5FEA2D, 0x7527BAC7EBE5F17B,
        0x3D0739F78A5292EA, 0x6BFB5FB11F8D5D08, 0x56033046FC7B6BAB, 0xF0CFBC209AF4361D,
        0xA9E391615EE61B08, 0x6599855F14A06840, 0x8DFFD8804D732731, 0x06061556CA73A8C9
    };

    int index = offset >> 6;
    int shift = offset & 63;

    if (shift) {
        segment[0] = bits[index + 1] >> (64 - shift) | bits[index] << shift;
        segment[1] = bits[index + 2] >> (64 - shift) | bits[index + 1] << shift;
        segment[2] = bits[index + 3] >> (64 - shift) | bits[index + 2] << shift;
    }
    else {
        segment[0] = bits[index];
        segment[1] = bits[index + 1];
        segment[2] = bits[index + 2];
    }
}

int64_t _right(double y[static 2], unsigned __int128 frac)
{
    /* Bits of π */
    const uint64_t p[] = { 0xC4C6628B80DC1CD1, 0xC90FDAA22168C234 };

    uint64_t q1 = frac >> 64;
    uint64_t q0 = frac;
    int64_t shift = __builtin_clzll(q1);

    if (shift) {
        q1 = q1 << shift | q0 >> (64 - shift);
        q0 <<= shift;
    }

    unsigned __int128 r = _umuldi(p[1], q1) + (_umuldi(p[1], q0) >> 64) + (_umuldi(p[0], q1) >> 64);
    uint64_t r1 = r >> 64;
    uint64_t r0 = r;

    y[0] = r1;
    y[1] = (int64_t)(r1 - ((uint64_t)y[0])) + 0x1p-64 * r0;

    return shift;
}

/* Argument reduction for trigonometric functions
 *
 * The prototype of this function resembles `__rem_pio2` in GCC, but this
 * function is only for `double`.  Pseudocode is as follows.
 *
 * quotient = nearest integer of x / (π/2)
 * *y = IEEE remainder of x / (π/2) = x - quotient * (π/2)
 *
 * return quotient with accurate sign and last 2 bits
 */
int __rem_pio2(double x, double y[static 2])
{
    int64_t i = reinterpret(int64_t, x);
    int64_t magnitude = i & 0x7FFFFFFFFFFFFFFF;

    if (magnitude < 0x41B921FB54442D18)
        return _small(x, y);

    if (magnitude >= 0x7FF0000000000000) {
        *y = x - x;
        return 0;
    }

    uint64_t significand = (i & 0x000FFFFFFFFFFFFF) | 0x0010000000000000;
    uint64_t segment[3];
    _segment(segment, (magnitude >> 52) - 1077);

    unsigned __int128 product
        = ((unsigned __int128)(segment[0] * significand) << 64)
        + _umuldi(segment[1], significand)
        + (_umuldi(segment[2], significand) >> 64);

    __int128 r = product << 2;
    __int128 s = r >> 127;
    int q = (product >> 126) - s;

    uint64_t shifter = 0x43E0000000000000 - (_right(y, (r + s) ^ s) << 52);
    uint64_t signbit = (i ^ (int64_t)(r >> 64)) & 0x8000000000000000;
    double coeff = reinterpret(double, shifter | signbit);

    y[0] *= coeff;
    y[1] *= coeff;

    return i < 0 ? -q : q;
}
