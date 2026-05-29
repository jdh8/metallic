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

/* Metallic vs system libm.  One row: function, the two sweep times (seconds),
 * and the libm/metallic ratio (>1 => Metallic faster).  See bench/README.md. */
#define BENCH(f) int main(void) {                             \
    double m = bench(metallic_##f), l = bench(f);             \
    printf("%-7s metallic %8.4f  libm %8.4f  libm/m %5.2f\n", \
           #f, m, l, l / m);                                  \
}

#ifndef BENCH_LABEL
#define BENCH_LABEL "?"
#endif

/* Metallic vs system libm vs CORE-MATH (cr_*).  One self-describing row: model
 * label, function, the three sweep times (s), and the cr/metallic ratio
 * (>1 => Metallic faster). */
#define BENCH_CR(f) int main(void) {                                      \
    double m = bench(metallic_##f), l = bench(f), c = bench(cr_##f);      \
    printf("%-6s %-7s metallic %8.4f  libm %8.4f  core-math %8.4f  cr/m %5.2f\n", \
           BENCH_LABEL, #f, m, l, c, c / m);                             \
}
