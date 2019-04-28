#include "src/math/reinterpret.h"
#include <time.h>
#include <stdint.h>

static double bench(float f(float))
{
    volatile float dummy;
    clock_t start = clock();

    for (uint32_t i = 0x7FF00000; i <= 0xFF800000; i += 19) {
        double x = reinterpret(float, i);
        dummy = f(x);
        dummy = f(-x);
    }

    return (double)(clock() - start) / CLOCKS_PER_SEC;
    dummy;
}

#define BENCH(f) int main(void) { printf("%9f\n%9f\n", bench(metallic_##f), bench(f)); }
