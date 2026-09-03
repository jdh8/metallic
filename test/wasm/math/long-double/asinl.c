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
    const unsigned __int128 one = (unsigned __int128)0x3fff << 112;
    const unsigned __int128 pi = U128_(0x4000921fb54442d1, 0x8469898cc51701b8);
    const unsigned __int128 pi_2 = U128_(0x3fff921fb54442d1, 0x8469898cc51701b8);
    const unsigned __int128 pi_6 = U128_(0x3ffe0c152382d736, 0x58465bb32e0f567b);
    const unsigned __int128 pi_3 = U128_(0x3fff0c152382d736, 0x58465bb32e0f567b);

    assert(bits_(asinl(0.0L)) == 0);
    assert(bits_(asinl(-0.0L)) == sign);
    assert(bits_(asinl(1.0L)) == pi_2);
    assert(bits_(asinl(-1.0L)) == (sign | pi_2));
    assert(bits_(asinl(0.5L)) == pi_6);
    assert(bits_(asinl(-0.5L)) == (sign | pi_6));
    assert(bits_(asinl(0.75L))
        == U128_(0x3ffeb235315c680d, 0xc081583db360d5e2));
    assert(bits_(asinl(-0.375L))
        == U128_(0xbffd899f4edc962d, 0x304fa465c06df1b6));
    assert(bits_(asinl(LDBL_TRUE_MIN)) == 1);

    assert(bits_(acosl(0.0L)) == pi_2);
    assert(bits_(acosl(-0.0L)) == pi_2);
    assert(bits_(acosl(1.0L)) == 0);
    assert(bits_(acosl(-1.0L)) == pi);
    assert(bits_(acosl(0.5L)) == pi_3);
    assert(bits_(acosl(-0.5L))
        == U128_(0x40000c152382d736, 0x58465bb32e0f567b));
    assert(bits_(acosl(0.75L))
        == U128_(0x3ffe720a392c1d95, 0x4851badbd6cd2d8e));
    assert(bits_(acosl(-0.75L))
        == U128_(0x4000359d26f93b6c, 0x32551ad5cf63b655));
    assert(bits_(acosl(from_bits_(one - 1)))
        == U128_(0x3fc7000000000000, 0));

    assert(isnan(asinl(1.5L)));
    assert(isnan(asinl(-INFINITY)));
    assert(isnan(acosl(1.5L)));
    unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    unsigned __int128 quiet = (unsigned __int128)1 << 111;
    assert((bits_(asinl(from_bits_(snan))) & quiet) != 0);
    assert((bits_(acosl(from_bits_(snan))) & quiet) != 0);
}
