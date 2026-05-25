#include <math.h>
#include <assert.h>

int main(void)
{
    /* Cast-through-double stub — exact for values |x| < 2^53. */
    assert(nearbyintl(0.0L) == 0.0L);
    assert(nearbyintl(0.5L) == 0.0L);
    assert(nearbyintl(1.5L) == 2.0L);
    assert(nearbyintl(2.5L) == 2.0L);
    assert(nearbyintl(-1.5L) == -2.0L);
    assert(nearbyintl(-2.5L) == -2.0L);
    assert(nearbyintl(INFINITY) == (long double)INFINITY);
    assert(isnan(nearbyintl(NAN)));
}
