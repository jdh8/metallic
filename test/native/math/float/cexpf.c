#include "src/math/float/cexpf.c"
#include "src/math/float/rem_pio2f.c"
#include "complex.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex expz = cexpf(z);

    verify2(capprox(expz, cexp(z), 1, 1), x, y);
    verify2(cidentical(conjf(expz), cexpf(conjf(z))), x, y);
}

static void divergent(float x, float y)
{
    float _Complex upper = cexpf(CMPLXF(x, y));
    float _Complex lower = cexpf(CMPLXF(x, -y));

    verify2(isnan(crealf(upper)), x, y);
    verify2(isnan(cimagf(upper)), x, y);
    verify2(isnan(crealf(lower)), x, -y);
    verify2(isnan(cimagf(lower)), x, -y);
}

static void pole(float x)
{
    verify(isinf(crealf(cexpf(CMPLXF(INFINITY, x)))), x);
    verify(isnan(cimagf(cexpf(CMPLXF(INFINITY, x)))), x);
    verify(cexpf(CMPLXF(-INFINITY, x)) == 0, x);
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x00135769)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000; j += 0x00135769)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00123456)
            mirror(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000; j += 0x00135769) {
        float x = reinterpret(float, j);
        pole(x);
        pole(-x);
    }
}
