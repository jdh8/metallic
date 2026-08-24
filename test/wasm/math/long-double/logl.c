#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>

static _Bool same_(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

int main(void)
{
    assert(same_(logl(1.0L), 0.0L));
    assert(isinf(logl(0.0L)) && signbit(logl(0.0L)));
    assert(isinf(logl(-0.0L)) && signbit(logl(-0.0L)));
    assert(isnan(logl(-1.0L)));
    assert(isinf(logl(INFINITY)) && !signbit(logl(INFINITY)));
    assert(isnan(logl(NAN)));

    const unsigned __int128 snan = (unsigned __int128)0x7fff << 112 | 1;
    const unsigned __int128 quiet = (unsigned __int128)1 << 111;
    assert((reinterpret(unsigned __int128,
        logl(reinterpret(long double, snan))) & quiet) != 0);
}
