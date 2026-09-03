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
    assert(same_bits_(log1pl(0.0L), 0));
    assert(same_bits_(log1pl(-0.0L), (u128)1 << 127));
    assert(isinf(log1pl(-1.0L)) && signbit(log1pl(-1.0L)));
    assert(isnan(log1pl(-1.5L)));
    assert(isnan(log1pl(-INFINITY)));
    assert(isinf(log1pl(INFINITY)) && !signbit(log1pl(INFINITY)));
    assert(isnan(log1pl(NAN)));

    assert(same_bits_(log1pl(1.0L),
        U128_(0x3ffe62e42fefa39e, 0xf35793c7673007e6)));
    assert(same_bits_(log1pl(3.0L),
        U128_(0x3fff62e42fefa39e, 0xf35793c7673007e6)));
    assert(same_bits_(log1pl(-0.5L),
        U128_(0xbffe62e42fefa39e, 0xf35793c7673007e6)));
    assert(same_bits_(log1pl(-0.75L),
        U128_(0xbfff62e42fefa39e, 0xf35793c7673007e6)));

    const u128 exact_tiny[] = {
        1,
        U128_(0x0001000000000000, 0),
        U128_(0x3f8d000000000000, 0),
        U128_(0xbf8d000000000000, 0),
        U128_(0x3f8e000000000000, 0),
        U128_(0xbf8e000000000000, 0),
    };
    for (unsigned i = 0; i < sizeof exact_tiny / sizeof exact_tiny[0]; ++i)
        assert(same_bits_(log1pl(from_bits_(exact_tiny[i])), exact_tiny[i]));

    u128 half_ulp = U128_(0x3f8f000000000000, 0);
    assert(same_bits_(log1pl(from_bits_(half_ulp)), half_ulp - 1));
    half_ulp |= (u128)1 << 127;
    assert(same_bits_(log1pl(from_bits_(half_ulp)), half_ulp + 1));

    static const u128 known[][2] = {
        {U128_(0x3fecffffffffffff, 0xffffffffffffffff),
         U128_(0x3fecffffc0000aaa, 0xa8aaab1110fbbbbf)},
        {U128_(0xbfecffffffffffff, 0xffffffffffffffff),
         U128_(0xbfed000020000555, 0x5655558888933335)},
        {U128_(0x3fed000000000000, 0),
         U128_(0x3fecffffc0000aaa, 0xa8aaab1110fbbbc0)},
        {U128_(0xbfed000000000000, 0),
         U128_(0xbfed000020000555, 0x5655558888933335)},
        {U128_(0xbffeffffffffffff, 0xffffffffffffffff),
         U128_(0xc005394d72518e72, 0x52d35076091866f9)},
        {U128_(0x3ffeffffffffffff, 0xffffffffffffffff),
         U128_(0x3ffe62e42fefa39e, 0xf35793c7673007e5)},
        {U128_(0x40fe000000000000, 0),
         U128_(0x400661814bbfb3fb, 0x54643c339fc8d7de)},
        {U128_(0x40ff000000000000, 0),
         U128_(0x400662e42fefa39e, 0xf35793c7673007e6)},
        {U128_(0x3f9b000000000000, 0),
         U128_(0x3f9affffffffffff, 0xfffffffffffff000)},
        {U128_(0xbf9b000000000000, 0),
         U128_(0xbf9b000000000000, 0x0000000000000800)},
    };
    for (unsigned i = 0; i < sizeof known / sizeof known[0]; ++i)
        assert(same_bits_(log1pl(from_bits_(known[i][0])), known[i][1]));

    const u128 snan = ((u128)0x7fff << 112) | 1;
    assert((reinterpret(u128, log1pl(from_bits_(snan))) & ((u128)1 << 111))
        != 0);
}
