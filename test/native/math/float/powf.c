#include "src/math/float/powf.c"
#include "binary.h"
#include <assert.h>

static void swapped(float y, float x)
{
    verify2(faithful(powf(x, y), pow(x, y)), x, y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(swapped, reinterpret(float, j), reinterpret(float, i));

    for (uint32_t i = 0; i < 1 << 10; ++i) {
        float any = reinterpret(float, i << 22);
        verify(powf(1, any) == 1, any);
        verify(powf(any, 0) == 1, any);
        verify(powf(any, -0.0f) == 1, any);
    }

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00100000) {
        float x = reinterpret(float, i);
        mirror(swapped, x, -0.0f);
        mirror(swapped, x, -INFINITY);
        mirror(swapped, INFINITY, -x);
    }

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        for (int y = 0; y < 1024; ++y)
            quadrants(swapped, y, reinterpret(float, i));

    for (uint32_t i = 0x80000001; i < 0xFF800000; i += 0x00098765)
        for (float y = -1023.4; y < 1024; ++y)
            assert(isnan(powf(reinterpret(float, i), y)));
}
