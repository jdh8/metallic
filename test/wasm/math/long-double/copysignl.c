#include "src/math/reinterpret.h"
#include <math.h>
#include <assert.h>

static _Bool ident(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

int main(void)
{
    assert(ident(copysignl(1.0L, 2.0L), 1.0L));
    assert(ident(copysignl(1.0L, -2.0L), -1.0L));
    assert(ident(copysignl(-1.0L, 2.0L), 1.0L));
    assert(ident(copysignl(0.0L, -1.0L), -0.0L));
    assert(ident(copysignl(-0.0L, 1.0L), 0.0L));
    assert(copysignl(INFINITY, -1.0L) == -INFINITY);
    assert(isnan(copysignl(NAN, 1.0L)));

    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;
    const unsigned __int128 step = (unsigned __int128)0x000001918E475914 << 64 | 0x4C37F177F056B903;

    for (unsigned __int128 i = 0; i < inf; i += step) {
        long double x = reinterpret(long double, i);
        assert(ident(copysignl(x, 1.0L), x));
        assert(ident(copysignl(x, -1.0L), -x));
        assert(ident(copysignl(-x, 1.0L), x));
    }
}
