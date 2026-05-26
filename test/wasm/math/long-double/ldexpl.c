#include <math.h>
#include <float.h>
#include <assert.h>

int main(void)
{
    /* Trivial cases. */
    assert(ldexpl(1.0L, 0) == 1.0L);
    assert(ldexpl(1.0L, 10) == 1024.0L);
    assert(ldexpl(0.5L, 1) == 1.0L);
    assert(ldexpl(-1.0L, 3) == -8.0L);
    assert(ldexpl(0.0L, 100) == 0.0L);
    assert(isnan(ldexpl(NAN, 5)));

    /* Binary128 overflow / underflow boundaries. */
    assert(ldexpl(1.0L, 16384) == (long double)INFINITY);
    assert(ldexpl(-1.0L, 16384) == -(long double)INFINITY);
    assert(ldexpl(1.0L, -16495) == 0.0L);
    assert(ldexpl(1.0L, -16494) == LDBL_TRUE_MIN);

    /* Subnormal / normal junction. */
    assert(ldexpl(1.0L, -16382) == LDBL_MIN);
    assert(ldexpl(LDBL_TRUE_MIN, 16494) == 1.0L);
    assert(ldexpl(LDBL_MIN, -1) == LDBL_MIN / 2);  /* drops into subnormal */

    /* Agreement with double scalbn for values representable in double. */
    for (int n = -100; n <= 100; n += 5) {
        assert(ldexpl(1.0L, n) == (long double)scalbn(1.0, n));
        assert(ldexpl(3.0L, n) == (long double)scalbn(3.0, n));
        assert(ldexpl(-5.5L, n) == (long double)scalbn(-5.5, n));
    }
}
