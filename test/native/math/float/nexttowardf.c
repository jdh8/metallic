#include "src/math/float/nexttowardf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(identical(nexttowardf(1.0f, 1.0L), 1.0f));

    float up = nexttowardf(1.0f, 2.0L);
    assert(up > 1.0f);
    assert(up == nextafterf(1.0f, 2.0f));

    float down = nexttowardf(1.0f, 0.0L);
    assert(down < 1.0f);
    assert(down == nextafterf(1.0f, 0.0f));

    assert(nexttowardf(0.0f, 1.0L) > 0.0f);
    assert(nexttowardf(0.0f, -1.0L) < 0.0f);

    /* NaN. */
    assert(isnan(nexttowardf(NAN, 0.0L)));
    assert(isnan(nexttowardf(0.0f, (long double)NAN)));

    /* Long-double-precise direction smaller than a float ulp. */
    float one = 1.0f;
    long double above = nextafterl(1.0L, 2.0L);
    assert(above > 1.0L);
    float r = nexttowardf(one, above);
    assert(r > one);
    assert(r == nextafterf(one, 2.0f));
}
