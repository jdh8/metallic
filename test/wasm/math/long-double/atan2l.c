#include "src/math/reinterpret.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#define U128_(hi, lo) \
    ((unsigned __int128)(uint64_t)(hi) << 64 | (unsigned __int128)(uint64_t)(lo))

static long double from_bits_(unsigned __int128 bits)
{
    return reinterpret(long double, bits);
}

static _Bool same_(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

int main(void)
{
    const long double pi = from_bits_(U128_(0x4000921fb54442d1, 0x8469898cc51701b8));
    const long double pi_2 = from_bits_(U128_(0x3fff921fb54442d1, 0x8469898cc51701b8));
    const long double pi_4 = from_bits_(U128_(0x3ffe921fb54442d1, 0x8469898cc51701b8));
    const long double pi_3_4 = from_bits_(U128_(0x40002d97c7f3321d, 0x234f272993d1414a));

    assert(same_(atan2l(0.0L, 0.0L), 0.0L));
    assert(same_(atan2l(0.0L, -0.0L), pi));
    assert(same_(atan2l(0.0L, INFINITY), 0.0L));
    assert(same_(atan2l(0.0L, -INFINITY), pi));

    assert(same_(atan2l(-0.0L, 0.0L), -0.0L));
    assert(same_(atan2l(-0.0L, -0.0L), -pi));
    assert(same_(atan2l(-0.0L, INFINITY), -0.0L));
    assert(same_(atan2l(-0.0L, -INFINITY), -pi));

    assert(same_(atan2l(INFINITY, 0.0L), pi_2));
    assert(same_(atan2l(INFINITY, -0.0L), pi_2));
    assert(same_(atan2l(INFINITY, INFINITY), pi_4));
    assert(same_(atan2l(INFINITY, -INFINITY), pi_3_4));

    assert(same_(atan2l(-INFINITY, 0.0L), -pi_2));
    assert(same_(atan2l(-INFINITY, -0.0L), -pi_2));
    assert(same_(atan2l(-INFINITY, INFINITY), -pi_4));
    assert(same_(atan2l(-INFINITY, -INFINITY), -pi_3_4));

    assert(same_(atan2l(1.0L, INFINITY), 0.0L));
    assert(same_(atan2l(-1.0L, INFINITY), -0.0L));
    assert(same_(atan2l(1.0L, -INFINITY), pi));
    assert(same_(atan2l(-1.0L, -INFINITY), -pi));
    assert(same_(atan2l(LDBL_TRUE_MIN, LDBL_MAX), 0.0L));
    assert(same_(atan2l(-LDBL_TRUE_MIN, LDBL_MAX), -0.0L));
    assert(same_(atan2l(1.0L, 2.0L),
        from_bits_(U128_(0x3ffddac670561bb4, 0xf68adfc88bd97875))));
    assert(same_(atan2l(2.0L, 1.0L),
        from_bits_(U128_(0x3fff1b6e192ebbe4, 0x46c6d19aa220a39b))));
    assert(same_(atan2l(3.0L, -7.0L),
        from_bits_(U128_(0x40005e4c36ca0118, 0xa2bf4259eee68f07))));
    assert(isnan(atan2l(NAN, 1.0L)));
    assert(isnan(atan2l(1.0L, NAN)));

    const unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    const unsigned __int128 quiet = (unsigned __int128)1 << 111;
    long double signaling = from_bits_(snan);
    assert((reinterpret(unsigned __int128, atan2l(signaling, 1.0L)) & quiet) != 0);
    assert((reinterpret(unsigned __int128, atan2l(1.0L, signaling)) & quiet) != 0);
}
