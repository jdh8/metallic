#include "src/math/float/cacoshf.c"
#include "complex.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex acoshz = cacoshf(z);

    verify2(capprox(acoshz, cacosh(z), 1, 1), x, y);
    verify2(cidentical(conjf(acoshz), cacoshf(conjf(z))), x, y);
}

static void divergent(float x, float y)
{
    float _Complex z = cacoshf(CMPLXF(x, y));

    verify2(isnan(crealf(z)), x, y);
    verify2(isnan(cimagf(z)), x, y);
}

static void realnan(float x, float y)
{
    float _Complex z = cacoshf(CMPLXF(x, y));

    verify2(isnan(crealf(z)), x, y);
    verify2(fabsf(cimagf(z)) == asinf(1), x, y);
}

static void pole(float x, float y)
{
    float _Complex z = cacoshf(CMPLXF(x, y));

    verify2(isinf(crealf(z)), x, y);
    verify2(isnan(cimagf(z)), x, y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7FC00000; j < 0x80000000; j += 0x00135769)
        for (uint32_t i = 1; i < 0x7F800000; i += 0x00123456)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0; j < 0x7F800000; j += 0x00135769)
        for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 0x00123456)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 0x00123456)
        quadrants(realnan, 0, reinterpret(float, i));

    for (uint32_t j = 0x7FC00000; j < 0x80000000; j += 0x00135769)
        quadrants(pole, INFINITY, reinterpret(float, j));
}
