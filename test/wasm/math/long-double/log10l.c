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
    assert(same_bits_(log10l(1.0L), 0));
    assert(isinf(log10l(0.0L)) && signbit(log10l(0.0L)));
    assert(isinf(log10l(-0.0L)) && signbit(log10l(-0.0L)));
    assert(isnan(log10l(-1.0L)));
    assert(isinf(log10l(INFINITY)) && !signbit(log10l(INFINITY)));
    assert(isnan(log10l(NAN)));

    assert(same_bits_(log10l(2.0L),
        U128_(0x3ffd34413509f79f, 0xef311f12b35816f9)));

    long double power = 1.0L;
    for (int k = 0; k <= 48; ++k) {
        assert(log10l(power) == (long double)k);
        power *= 10.0L;
    }

    u128 m = ((u128)1 << 112) - ((u128)1 << 34);
    assert(same_bits_(log10l(from_bits_(((u128)16383 << 112) | m)),
        U128_(0x3ffd34413509f79f, 0xef311f0f39e8b454)));
    assert(same_bits_(log10l(from_bits_(((u128)1 << 78) - 1)),
        U128_(0xc00b34db55a47c9b, 0xbf28b7a23ccd8e80)));
    assert(same_bits_(log10l(from_bits_(1)),
        U128_(0xc00b3653051d20c1, 0x8a143b801b7c5661)));
    assert(same_bits_(log10l(from_bits_(
        U128_(0x7ffeffffffffffff, 0xffffffffffffffff))),
        U128_(0x400b34413509f79f, 0xef311f12b35816f9)));

    assert(same_bits_(log10l(from_bits_(
        U128_(0x3fff000000000000, 0x0000000000000001))),
        U128_(0x3f8dbcb7b1526e50, 0xe32a6ab7555f5a67)));
    assert(same_bits_(log10l(from_bits_(
        U128_(0x3ffeffffffffffff, 0xfffffffffffffffe))),
        U128_(0xbf8dbcb7b1526e50, 0xe32a6ab7555f5a69)));

    const u128 snan = ((u128)0x7fff << 112) | 1;
    assert((reinterpret(u128, log10l(from_bits_(snan))) & ((u128)1 << 111))
        != 0);
}
