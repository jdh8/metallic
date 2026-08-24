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
    assert(hypotl(3.0L, -4.0L) == 5.0L);
    assert(hypotl(-1.0L, 0.0L) == 1.0L);
    assert(same_(hypotl(-0.0L, -0.0L), 0.0L));
    assert(isinf(hypotl(LDBL_MAX, LDBL_MAX)));
    assert(hypotl(LDBL_MIN, LDBL_TRUE_MIN) == LDBL_MIN);

    unsigned __int128 quiet_nan = (unsigned __int128)0x7fff8 << 108;
    unsigned __int128 signaling_nan = (unsigned __int128)0x7fff << 112 | 1;
    long double qnan = reinterpret(long double, quiet_nan);
    long double snan = reinterpret(long double, signaling_nan);
    assert(isinf(hypotl(INFINITY, qnan)));
    assert(isinf(hypotl(qnan, -INFINITY)));
    assert(isnan(hypotl(INFINITY, snan)));
    assert(isnan(hypotl(snan, -INFINITY)));
    assert((reinterpret(unsigned __int128, hypotl(INFINITY, snan))
        & ((unsigned __int128)1 << 111)) != 0);

    assert(same_(hypotl(reinterpret(long double, (unsigned __int128)3),
        reinterpret(long double, (unsigned __int128)4)),
        reinterpret(long double, (unsigned __int128)5)));
}
