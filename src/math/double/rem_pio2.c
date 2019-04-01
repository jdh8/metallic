#include "../../soft/integer/kernel/umuldi.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Argument reduction for |x| < π * 0x1p27 */
static int _small(double x, double y[static 2])
{
    const double pi_2[] = { 0x1.921fb5p0, 0x1.110b46p-26, 0x1.1a62633145c07p-54 };
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

/* Get 192 bits of 0x1p-31 / π with `offset` bits skipped */
static void _segment(uint64_t segment[static 3], int offset)
{
    const uint64_t bits[] = {
        0x00000000A2F9836E, 0x4E441529FC2757D1, 0xF534DDC0DB629599, 0x3C439041FE5163AB,
        0xDEBBC561B7246E3A, 0x424DD2E006492EEA, 0x09D1921CFE1DEB1C, 0xB129A73EE88235F5,
        0x2EBB4484E99C7026, 0xB45F7E413991D639, 0x835339F49C845F8B, 0xBDF9283B1FF897FF,
        0xDE05980FEF2F118B, 0x5A0A6D1F6D367ECF, 0x27CB09B74F463F66, 0x9E5FEA2D7527BAC7,
        0xEBE5F17B3D0739F7, 0x8A5292EA6BFB5FB1, 0x1F8D5D0856033046, 0xFC7B6BABF0CFBC20,
        0x9AF4361DA9E39161, 0x5EE61B086599855F, 0x14A068408DFFD880, 0x4D73273106061557
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

/* Helper function to eventually get bits of π/2 * |x|
 *
 * y = π/4 * (frac << clz(frac) >> 11)
 * return clz(frac)
 *
 * Right shift 11 bits to make upper half fit in `double`
 */
int64_t _right(unsigned __int128 frac, double y[static 2])
{
    /* Bits of π/4 */
    const uint64_t p[] = { 0xC4C6628B80DC1CD1, 0xC90FDAA22168C234 };

    uint64_t q1 = frac >> 64;
    uint64_t q0 = frac;
    int64_t shift = __builtin_clzll(q1);

    q1 = q1 << shift | q0 >> (64 - shift);
    q0 <<= shift;

    unsigned __int128 r = (_umuldi(p[1], q1) >> 11) + (uint64_t)(0x1p-75 * p[0] * q1 + 0x1p-75 * p[1] * q0);

    y[0] = r >> 64;
    y[1] = 0x1p-64 * (uint64_t)r;

    return shift;
}

/* Argument reduction for trigonometric functions
 *
 * The prototype of this function resembles `__rem_pio2` in GCC, but this
 * function is only for `double`.  Pseudocode is as follows.
 *
 * quotient = nearest integer of x / (π/2)
 * y = IEEE remainder of x / (π/2) = x - quotient * (π/2)
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
    _segment(segment, (magnitude >> 52) - 1045);

    /* First 128 bits of fractional part of x/(2π) */
    unsigned __int128 product
        = ((unsigned __int128)(segment[0] * significand) << 64)
        + _umuldi(segment[1], significand)
        + (segment[2] >> 32) * (significand >> 32);

    __int128 r = product << 2;
    __int128 s = r >> 127;
    int q = (product >> 126) - s;

    uint64_t shifter = 0x3CB0000000000000 - (_right(r ^ s, y) << 52);
    uint64_t signbit = (i ^ (int64_t)(r >> 64)) & 0x8000000000000000;
    double coeff = reinterpret(double, shifter | signbit);

    y[0] *= coeff;
    y[1] *= coeff;

    return i < 0 ? -q : q;
}
