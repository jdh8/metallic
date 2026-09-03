#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

#define U128_(hi, lo) \
    ((unsigned __int128)(uint64_t)(hi) << 64 | (unsigned __int128)(uint64_t)(lo))

static long double from_bits_(unsigned __int128 bits)
{
    return reinterpret(long double, bits);
}

static _Bool same_bits_(long double value, unsigned __int128 bits)
{
    return reinterpret(unsigned __int128, value) == bits;
}

int main(void)
{
    const unsigned __int128 one = U128_(0x3fff000000000000, 0);
    const unsigned __int128 infinity = U128_(0x7fff000000000000, 0);
    const unsigned __int128 sign = (unsigned __int128)1 << 127;

    assert(same_bits_(powl(NAN, 0.0L), one));
    assert(same_bits_(powl(1.0L, NAN), one));
    assert(isnan(powl(-2.0L, 0.5L)));
    assert(same_bits_(powl(-0.0L, 3.0L), sign));
    assert(same_bits_(powl(-0.0L, -3.0L), sign | infinity));
    assert(powl(-2.0L, 3.0L) == -8.0L);
    assert(powl(2.0L, 10.0L) == 1024.0L);
    assert(same_bits_(powl(2.0L, -16494.0L), 1));
    assert(same_bits_(powl(2.0L, -16495.0L), 0));

    struct {
        unsigned __int128 x;
        unsigned __int128 y;
        unsigned __int128 result;
    } known[] = {
        {U128_(0x4002400000000000, 0), U128_(0x3ffe000000000000, 0),
         U128_(0x400094c583ada5b5, 0x29204a2bc830cd9c)},
        {U128_(0x3fff000000000000, 1), U128_(0x4063000000000000, 0),
         U128_(0x3fff0010008002aa, 0xb5557777d27df7e1)},
        {U128_(0x3ffeffffffffffff, UINT64_MAX), U128_(0xc06d000000000000, 0),
         U128_(0x3fff2216045b6f5c, 0xcf9ced688384e06c)},
        {U128_(0x4000800000000000, 0), U128_(0x3ffd555555555555, 0x5555555555555555),
         U128_(0x3fff7137449123ef, 0x65cdde7f16c56e32)},
    };
    for (unsigned i = 0; i < sizeof known / sizeof *known; ++i)
        assert(same_bits_(powl(from_bits_(known[i].x), from_bits_(known[i].y)),
            known[i].result));
}
