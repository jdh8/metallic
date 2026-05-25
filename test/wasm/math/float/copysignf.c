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
    assert(ident(copysignf(1.0f, 2.0f), 1.0f));
    assert(ident(copysignf(1.0f, -2.0f), -1.0f));
    assert(ident(copysignf(0.0f, -1.0f), -0.0f));
    assert(ident(copysignf(-0.0f, 1.0f), 0.0f));
    assert(copysignf(INFINITY, -1.0f) == -INFINITY);
    assert(isnan(copysignf(NAN, 1.0f)));

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765) {
        float x = reinterpret(float, i);
        assert(ident(copysignf(x, 1.0f), x));
        assert(ident(copysignf(x, -1.0f), -x));
    }
}
