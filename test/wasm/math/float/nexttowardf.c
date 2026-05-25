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
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765) {
        float x = reinterpret(float, i);
        if (x == 0) continue;
        float up = nextafterf(x, INFINITY);
        assert(ident(nexttowardf(x, (long double)up * 2), up));
    }

    assert(ident(nexttowardf(1.0f, 1.0L), 1.0f));
    assert(nexttowardf(1.0f, 2.0L) > 1.0f);
    assert(nexttowardf(1.0f, 0.0L) < 1.0f);
    assert(nexttowardf(0.0f, 1.0L) > 0.0f);
    assert(isnan(nexttowardf(NAN, 0.0L)));
}
