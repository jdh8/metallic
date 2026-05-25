/* ldexpf is provided as an alias of scalbnf by src/math/float/scalbnf.c. */
#include "src/math/float/scalbnf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(ldexpf(1.0f, 0) == 1.0f);
    assert(ldexpf(1.0f, 1) == 2.0f);
    assert(ldexpf(1.0f, 10) == 1024.0f);
    assert(ldexpf(0.5f, 1) == 1.0f);
    assert(ldexpf(-1.0f, 3) == -8.0f);
    assert(ldexpf(0.0f, 100) == 0.0f);
    assert(identical(ldexpf(-0.0f, 100), -0.0f));

    /* Underflow to subnormal, then to zero. */
    assert(ldexpf(1.0f, -149) > 0.0f);
    assert(ldexpf(1.0f, -150) == 0.0f);

    /* Overflow to infinity. */
    assert(ldexpf(1.0f, 128) == (float)INFINITY);

    assert(isnan(ldexpf((float)NAN, 5)));

    /* Match scalbnf on a sweep. */
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00010CDF) {
        float x = reinterpret(float, i);
        for (int n = -30; n <= 30; n += 10)
            verify(identical(ldexpf(x, n), scalbnf(x, n)), x);
    }
}
