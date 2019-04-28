#include "src/math/reinterpret.h"
#include <time.h>
#include <stdint.h>

static double bench(double f(double))
{
    volatile double dummy;
    clock_t start = clock();

    for (uint64_t i = 0x7FF8000000000000; i <= 0xFFF0000000000000; i += 0x0000000893205C62) {
        double x = reinterpret(double, i);
        dummy = f(x);
        dummy = f(-x);
    }

    return (double)(clock() - start) / CLOCKS_PER_SEC;
    dummy;
}

#define BENCH(f) int main(void) { printf("%9f\n%9f\n", bench(metallic_##f), bench(f)); }
