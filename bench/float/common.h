#include "src/math/reinterpret.h"
#include <time.h>
#include <stdint.h>

static double bench(float f(float), uint32_t step)
{
    volatile float dummy;
    clock_t start = clock();

    for (uint32_t i = 0x7FF00000; i <= 0xFF800000; i += step) {
        double x = reinterpret(float, i);
        dummy = f(x);
        dummy = f(-x);
    }

    return (double)(clock() - start) / CLOCKS_PER_SEC;
    dummy;
}
