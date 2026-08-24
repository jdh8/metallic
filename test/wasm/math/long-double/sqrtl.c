#include "src/math/reinterpret.h"
#include <assert.h>
#include <float.h>
#include <math.h>

static _Bool same_(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

int main(void)
{
    assert(same_(sqrtl(0.0L), 0.0L));
    assert(same_(sqrtl(-0.0L), -0.0L));
    assert(sqrtl(4.0L) == 2.0L);
    assert(sqrtl(LDBL_TRUE_MIN) == ldexpl(1.0L, -8247));
    assert(isinf(sqrtl(INFINITY)) && !signbit(sqrtl(INFINITY)));
    assert(isnan(sqrtl(-1.0L)));

    assert(rsqrtl(4.0L) == 0.5L);
    assert(rsqrtl(LDBL_TRUE_MIN) == ldexpl(1.0L, 8247));
    assert(isinf(rsqrtl(0.0L)) && !signbit(rsqrtl(0.0L)));
    assert(isinf(rsqrtl(-0.0L)) && signbit(rsqrtl(-0.0L)));
    assert(rsqrtl(INFINITY) == 0.0L);
    assert(isnan(rsqrtl(-INFINITY)));

    assert(cbrtl(8.0L) == 2.0L);
    assert(cbrtl(-8.0L) == -2.0L);
    assert(cbrtl(LDBL_TRUE_MIN) == ldexpl(1.0L, -5498));
    assert(same_(cbrtl(-0.0L), -0.0L));
    assert(isinf(cbrtl(-INFINITY)) && signbit(cbrtl(-INFINITY)));

    const unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    unsigned __int128 quiet = (unsigned __int128)1 << 111;
    assert((reinterpret(unsigned __int128,
        sqrtl(reinterpret(long double, snan))) & quiet) != 0);
    assert((reinterpret(unsigned __int128,
        rsqrtl(reinterpret(long double, snan))) & quiet) != 0);
    assert((reinterpret(unsigned __int128,
        cbrtl(reinterpret(long double, snan))) & quiet) != 0);
}
