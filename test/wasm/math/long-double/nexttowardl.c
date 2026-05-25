#include "src/math/reinterpret.h"
#include <math.h>
#include <float.h>
#include <assert.h>

static _Bool ident(long double x, long double y)
{
    return reinterpret(unsigned __int128, x) == reinterpret(unsigned __int128, y);
}

int main(void)
{
    /* Same-value short-circuit. */
    assert(ident(nexttowardl(1.0L, 1.0L), 1.0L));
    assert(ident(nexttowardl(0.0L, 0.0L), 0.0L));

    /* Zero -> smallest subnormal. */
    long double up_from_zero = nexttowardl(0.0L, 1.0L);
    assert(up_from_zero > 0.0L);
    assert(up_from_zero == LDBL_TRUE_MIN);

    long double dn_from_zero = nexttowardl(0.0L, -1.0L);
    assert(dn_from_zero < 0.0L);

    /* Stepping must be a single ulp in binary128: incrementing the
     * 128-bit bit pattern of 1.0L. */
    long double up = nexttowardl(1.0L, 2.0L);
    unsigned __int128 a = reinterpret(unsigned __int128, 1.0L);
    assert(ident(up, reinterpret(long double, a + 1)));

    long double dn = nexttowardl(1.0L, 0.0L);
    assert(ident(dn, reinterpret(long double, a - 1)));

    /* NaN propagation. */
    assert(isnan(nexttowardl(NAN, 0.0L)));
    assert(isnan(nexttowardl(0.0L, (long double)NAN)));
}
