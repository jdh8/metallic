#include "src/math/float/atan2f.c"
#include "binary.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float angle = atan2f(y, x);

    verify2(faithful(angle, atan2(y, x)), x, y);
    verify2(identical(-angle, atan2f(-y, x)), x, y);
}

static void divergent(float x, float y)
{
    verify2(isnan(atan2f(x, y)), x, y);
    verify2(isnan(atan2f(y, x)), x, y);
}

int main(void)
{
    assert(atan2f(INFINITY, INFINITY) == (float)atan(1));

    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7FC00000; j < 0x80000000; j += 0x000ABCDE)
        for (uint32_t i = 0; j < 0x80000000; j += 0x00100000)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));
}
