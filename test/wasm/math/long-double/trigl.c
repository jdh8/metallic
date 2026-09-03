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

static void check_(long double x, unsigned __int128 sine,
    unsigned __int128 cosine, unsigned __int128 tangent)
{
    assert(bits_(sinl(x)) == sine);
    assert(bits_(cosl(x)) == cosine);
    assert(bits_(tanl(x)) == tangent);
}

int main(void)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    const unsigned __int128 one = U128_(0x3fff000000000000, 0);
    const unsigned __int128 tiny = (unsigned __int128)(0x3fff - 57) << 112;

    assert(bits_(sinl(0.0L)) == 0);
    assert(bits_(sinl(-0.0L)) == sign);
    assert(bits_(cosl(0.0L)) == one);
    assert(bits_(cosl(-0.0L)) == one);
    assert(bits_(tanl(0.0L)) == 0);
    assert(bits_(tanl(-0.0L)) == sign);
    assert(isnan(sinl(INFINITY)));
    assert(isnan(cosl(-INFINITY)));
    assert(isnan(tanl(INFINITY)));

    const unsigned __int128 small[] = {
        1,
        (unsigned __int128)1 << 112,
        (unsigned __int128)(0x3fff - 58) << 112,
        tiny - 1,
    };
    for (unsigned i = 0; i < sizeof small / sizeof *small; ++i) {
        long double x = from_bits_(small[i]);
        assert(bits_(sinl(x)) == small[i]);
        assert(bits_(sinl(-x)) == (small[i] | sign));
        assert(bits_(cosl(x)) == one);
        assert(bits_(tanl(x)) == small[i]);
        assert(bits_(tanl(-x)) == (small[i] | sign));
    }

    check_(1.0L,
        U128_(0x3ffeaed548f090ce, 0xe0418dd3d2138a1e),
        U128_(0x3ffe14a280fb5068, 0xb923848cdb2ed0e3),
        U128_(0x3fff8eb245cbee3a, 0x5b8acc7d41323141));

    const long double pi_2 = from_bits_(
        U128_(0x3fff921fb54442d1, 0x8469898cc51701b8));
    check_(pi_2,
        one,
        U128_(0x3f8ccd129024e088, 0xa67cc74020bbea64),
        U128_(0x40711c46bd572779, 0x93a2ee60193c957b));

    const long double pi = from_bits_(
        U128_(0x4000921fb54442d1, 0x8469898cc51701b8));
    check_(pi,
        U128_(0x3f8dcd129024e088, 0xa67cc74020bbea64),
        one | sign,
        U128_(0xbf8dcd129024e088, 0xa67cc74020bbea64));

    check_(from_bits_((unsigned __int128)(0x3fff - 10) << 112),
        U128_(0x3ff4fffffaaaaaae, 0xeeeeed4ed4edab4c),
        U128_(0x3ffefffff0000015, 0x555549f49f4d34d3),
        U128_(0x3ff5000005555577, 0x777854854dedc28f));

    check_(from_bits_((unsigned __int128)(0x3fff + 100) << 112),
        U128_(0xbffebe8ed97ac1f5, 0x8bea99cc1e489ea6),
        U128_(0x3ffdf4eb3ff66e36, 0xcd2dc761b1ffbcc6),
        U128_(0xbfffc86fbfa8cecc, 0x31efc7a12115f421));

    /* Medium cancellation in the fast Payne-Hanek reduction. */
    check_(from_bits_(U128_(0x409d4fb51e8fc339, 0x9f199688d9b9a19c)),
        U128_(0x3fc6921fb54442d1, 0x855478773135ec78),
        U128_(0x3ffeffffffffffff, 0xffffffffffffffff),
        U128_(0x3fc6921fb54442d1, 0x855478773135ec78));

    check_(LDBL_MAX,
        U128_(0x3ffee761623db0b6, 0xffc87a2204a2b017),
        U128_(0xbffd39b7257ed4a6, 0xf0d97d1cb93ef07e),
        U128_(0xc0008db7162c7114, 0x540eca807fae7391));

    assert(bits_(sinl(-1.0L))
        == (U128_(0x3ffeaed548f090ce, 0xe0418dd3d2138a1e) | sign));
    assert(bits_(cosl(-1.0L))
        == U128_(0x3ffe14a280fb5068, 0xb923848cdb2ed0e3));
    assert(bits_(tanl(-1.0L))
        == (U128_(0x3fff8eb245cbee3a, 0x5b8acc7d41323141) | sign));

    const unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    const unsigned __int128 quiet = (unsigned __int128)1 << 111;
    long double signaling = from_bits_(snan);
    assert(bits_(sinl(signaling)) == (snan | quiet));
    assert(bits_(cosl(signaling)) == (snan | quiet));
    assert(bits_(tanl(signaling)) == (snan | quiet));
}
