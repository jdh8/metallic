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
    /* Alias of rintf — must agree bitwise. */
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765) {
        float x = reinterpret(float, i);
        assert(ident(nearbyintf(x), rintf(x)));
        assert(ident(nearbyintf(-x), rintf(-x)));
    }

    assert(nearbyintf(0.5f) == 0.0f);
    assert(nearbyintf(1.5f) == 2.0f);
    assert(nearbyintf(2.5f) == 2.0f);
    assert(nearbyintf(-1.5f) == -2.0f);
    assert(isnan(nearbyintf(NAN)));
}
