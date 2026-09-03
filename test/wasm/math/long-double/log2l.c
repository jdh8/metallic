#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

typedef unsigned __int128 u128;

#define U128_(hi, lo) (((u128)(uint64_t)(hi) << 64) | (u128)(uint64_t)(lo))

static long double from_bits_(u128 bits)
{
    return reinterpret(long double, bits);
}

static _Bool same_bits_(long double x, u128 bits)
{
    return reinterpret(u128, x) == bits;
}

int main(void)
{
    assert(same_bits_(log2l(1.0L), 0));
    assert(isinf(log2l(0.0L)) && signbit(log2l(0.0L)));
    assert(isinf(log2l(-0.0L)) && signbit(log2l(-0.0L)));
    assert(isnan(log2l(-1.0L)));
    assert(isinf(log2l(INFINITY)) && !signbit(log2l(INFINITY)));
    assert(isnan(log2l(NAN)));

    const int powers[] = {-16494, -16382, -113, -1, 0, 1, 112, 16383};
    for (unsigned i = 0; i < sizeof powers / sizeof powers[0]; ++i) {
        long double x = ldexpl(1.0L, powers[i]);
        assert(log2l(x) == (long double)powers[i]);
    }

    u128 m = ((u128)1 << 112) - ((u128)1 << 34);
    assert(same_bits_(log2l(from_bits_(((u128)16383 << 112) | m)),
        U128_(0x3ffeffffffffffff, 0xfffffffa3aae26b5)));
    assert(same_bits_(log2l(from_bits_(((u128)1 << 78) - 1)),
        U128_(0xc00d008000000000, 0x0000000000171547)));

    assert(same_bits_(log2l(from_bits_(
        U128_(0x3fff000000000000, 0x0000000000000001))),
        U128_(0x3f8f71547652b82f, 0xe1777d0ffda0d23a)));
    assert(same_bits_(log2l(from_bits_(
        U128_(0x3ffeffffffffffff, 0xfffffffffffffffe))),
        U128_(0xbf8f71547652b82f, 0xe1777d0ffda0d23b)));

    const u128 snan = ((u128)0x7fff << 112) | 1;
    assert((reinterpret(u128, log2l(from_bits_(snan))) & ((u128)1 << 111))
        != 0);
}
