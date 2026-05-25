#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

static _Bool ident(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

int main(void)
{
    /* ldexpf is an alias of scalbnf. */
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765) {
        float x = reinterpret(float, i);
        for (int n = -30; n <= 30; n += 10)
            assert(ident(ldexpf(x, n), scalbnf(x, n)));
    }

    assert(ldexpf(1.0f, 10) == 1024.0f);
    assert(ldexpf(0.5f, 1) == 1.0f);
    assert(ldexpf(1.0f, 128) == (float)INFINITY);
    assert(ldexpf(1.0f, -150) == 0.0f);
    assert(isnan(ldexpf(NAN, 5)));
}
