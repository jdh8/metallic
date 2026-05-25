#include <math.h>
#include <assert.h>

int main(void)
{
    /* Cast-through-double stub: cover values for which the round trip
     * is exact. */
    assert(ldexpl(1.0L, 0) == 1.0L);
    assert(ldexpl(1.0L, 10) == 1024.0L);
    assert(ldexpl(0.5L, 1) == 1.0L);
    assert(ldexpl(-1.0L, 3) == -8.0L);
    assert(ldexpl(0.0L, 100) == 0.0L);
    assert(ldexpl(1.0L, 1024) == (long double)INFINITY);
    assert(ldexpl(1.0L, -1075) == 0.0L);
    assert(isnan(ldexpl(NAN, 5)));

    /* Agreement with double scalbn for values representable in double. */
    for (int n = -100; n <= 100; n += 5) {
        assert(ldexpl(1.0L, n) == (long double)scalbn(1.0, n));
        assert(ldexpl(3.0L, n) == (long double)scalbn(3.0, n));
        assert(ldexpl(-5.5L, n) == (long double)scalbn(-5.5, n));
    }
}
