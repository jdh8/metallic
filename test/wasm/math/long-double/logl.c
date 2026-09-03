#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

typedef unsigned __int128 u128;

#define U128_(hi, lo) (((u128)(uint64_t)(hi) << 64) | (u128)(uint64_t)(lo))

static _Bool same_(long double x, long double y)
{
    return reinterpret(u128, x) == reinterpret(u128, y);
}

static long double from_bits_(u128 bits)
{
    return reinterpret(long double, bits);
}

int main(void)
{
    assert(same_(logl(1.0L), 0.0L));
    assert(isinf(logl(0.0L)) && signbit(logl(0.0L)));
    assert(isinf(logl(-0.0L)) && signbit(logl(-0.0L)));
    assert(isnan(logl(-1.0L)));
    assert(isinf(logl(INFINITY)) && !signbit(logl(INFINITY)));
    assert(isnan(logl(NAN)));

    const u128 snan = (u128)0x7fff << 112 | 1;
    const u128 quiet = (u128)1 << 111;
    assert((reinterpret(u128,
        logl(reinterpret(long double, snan))) & quiet) != 0);

    assert(same_(logl(10.0L),
        from_bits_(U128_(0x400026bb1bbb5551, 0x582dd4adac5705a6))));
    assert(same_(logl(from_bits_(1)),
        from_bits_(U128_(0xc00c654628220780, 0x2c89d24d65e96274))));
    assert(same_(logl(from_bits_(
        U128_(0x7ffeffffffffffff, 0xffffffffffffffff))),
        from_bits_(U128_(0x400c62e42fefa39e, 0xf35793c7673007e6))));

    /* One ulp either side of 1 exercises cancellation in the accurate leg. */
    assert(same_(logl(from_bits_(
        U128_(0x3fff000000000000, 0x0000000000000001))),
        from_bits_(U128_(0x3f8effffffffffff, 0xffffffffffffffff))));
    assert(same_(logl(from_bits_(
        U128_(0x3ffeffffffffffff, 0xfffffffffffffffe))),
        from_bits_(U128_(0xbf8f000000000000, 0x0000000000000001))));

    /* Regression: squaring a tiny negative reduction must not wrap. */
    u128 m = ((u128)1 << 112) - ((u128)1 << 34);
    assert(same_(logl(from_bits_(((u128)16383 << 112) | m)),
        from_bits_(U128_(0x3ffe62e42fefa39e, 0xf35793c3673007e6))));
}
