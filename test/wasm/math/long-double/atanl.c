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

static unsigned __int128 bits_(long double x)
{
    return reinterpret(unsigned __int128, x);
}

int main(void)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    const unsigned __int128 pi_2 = U128_(0x3fff921fb54442d1, 0x8469898cc51701b8);
    const unsigned __int128 pi_4 = U128_(0x3ffe921fb54442d1, 0x8469898cc51701b8);

    assert(bits_(atanl(0.0L)) == 0);
    assert(bits_(atanl(-0.0L)) == sign);
    assert(bits_(atanl(1.0L)) == pi_4);
    assert(bits_(atanl(-1.0L)) == (sign | pi_4));
    assert(bits_(atanl(INFINITY)) == pi_2);
    assert(bits_(atanl(-INFINITY)) == (sign | pi_2));
    assert(bits_(atanl(0.5L))
        == U128_(0x3ffddac670561bb4, 0xf68adfc88bd97875));
    assert(bits_(atanl(2.0L))
        == U128_(0x3fff1b6e192ebbe4, 0x46c6d19aa220a39b));
    assert(bits_(atanl(LDBL_TRUE_MIN)) == 1);
    assert(bits_(atanl(-LDBL_TRUE_MIN)) == (sign | 1));

    unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    unsigned __int128 quiet = (unsigned __int128)1 << 111;
    assert((bits_(atanl(from_bits_(snan))) & quiet) != 0);
}
