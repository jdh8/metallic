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
    assert(expl(0.0L) == 1.0L);
    assert(expl(-INFINITY) == 0.0L);
    assert(isinf(expl(INFINITY)) && !signbit(expl(INFINITY)));
    assert(isnan(expl(NAN)));

    assert(exp2l(0.0L) == 1.0L);
    assert(exp2l(10.0L) == 1024.0L);
    assert(exp2l(-16494.0L) == LDBL_TRUE_MIN);
    assert(exp2l(-16495.0L) == 0.0L);
    assert(isinf(exp2l(16384.0L)));

    assert(exp10l(0.0L) == 1.0L);
    assert(exp10l(3.0L) == 1000.0L);
    assert(exp10l(-INFINITY) == 0.0L);

    assert(same_(expm1l(0.0L), 0.0L));
    assert(same_(expm1l(-0.0L), -0.0L));
    assert(expm1l(-INFINITY) == -1.0L);
    assert(isinf(expm1l(INFINITY)));
    assert(same_(expm1l(LDBL_TRUE_MIN), LDBL_TRUE_MIN));

    const unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    const unsigned __int128 quiet = (unsigned __int128)1 << 111;
    long double signaling = reinterpret(long double, snan);
    assert((reinterpret(unsigned __int128, expl(signaling)) & quiet) != 0);
    assert((reinterpret(unsigned __int128, exp2l(signaling)) & quiet) != 0);
    assert((reinterpret(unsigned __int128, exp10l(signaling)) & quiet) != 0);
    assert((reinterpret(unsigned __int128, expm1l(signaling)) & quiet) != 0);
}
