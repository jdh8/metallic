#include "src/math/reinterpret.h"
#include <math.h>
#include <assert.h>

static _Bool identical(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

static _Bool run(unsigned __int128 i)
{
    long double x = reinterpret(long double, i);
    long double y = sqrtl(x);

    unsigned __int128 j = reinterpret(unsigned __int128, y);

    long double prev = reinterpret(long double, j - 1);
    long double next = reinterpret(long double, j + 1);

    return reinterpret(unsigned __int128, y * prev) <= i && i <= reinterpret(unsigned __int128, y * next);
}

int main(void)
{
    assert(identical(0, sqrtl(0)));
    assert(identical(-0.0L, sqrtl(-0.0L)));
    assert(identical(INFINITY, sqrtl(INFINITY)));

    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;
    const unsigned __int128 step = (unsigned __int128)0x000001918E475914 << 64 | 0x4C37F177F056B903;

    for (unsigned __int128 i = 1; i < inf; i += step)
        assert(run(i));

    for (unsigned __int128 i = inf | 1; i > inf; i += step)
        assert(isnan(sqrtl(reinterpret(long double, i))));
}
