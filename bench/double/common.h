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

/* Metallic vs system libm.  One row: function, the two sweep times (seconds),
 * and the libm/metallic ratio (>1 => Metallic faster).  See bench/README.md. */
#define BENCH(f) int main(void) {                             \
    double m = bench(metallic_##f), l = bench(f);             \
    printf("%-7s metallic %8.4f  libm %8.4f  libm/m %5.2f\n", \
           #f, m, l, l / m);                                  \
}
