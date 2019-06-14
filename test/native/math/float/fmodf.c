#include "src/math/float/fmodf.h"
#include "binary.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float r = fmodf(x, y);

    verify2(identical(r, fmodf_(x, y)), x, y);
    verify2(identical(r, fmodf_(x, -y)), x, y);
    verify2(identical(-r, fmodf_(-x, y)), x, y);
    verify2(identical(-r, fmodf_(-x, -y)), x, y);
}

static void divergent(float x, float y)
{
    verify2(isnan(fmodf_(x, y)), x, y);
}

int main(void)
{
    for (uint32_t j = 6; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            convergent(reinterpret(float, i), reinterpret(float, j));

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        convergent(reinterpret(float, i), INFINITY);

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        quadrants(divergent, reinterpret(float, i), 0);

    for (uint32_t j = 0; j < 0x80000000; j += 0x00100000)
        for (uint32_t i = 0x7F800000; i < 0x80000000; i += 0x00098765)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));
}
