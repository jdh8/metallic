#include "common.h"
#include <math.h>
#include <stdio.h>

#define powf(x, y) metallic_powf(x, y)
#include "src/math/float/powf.c"
#undef powf

#ifdef BENCH_COREMATH
#include <binary32/pow/powf.c>   /* cr_powf */
#endif

static float bench_y_[BENCH_N];

static double bench2_(float f(float, float), BenchInterval ivx, BenchInterval ivy)
{
    rng_state = (uint32_t)time(NULL) + bench_call++;
    if (!rng_state) rng_state = 1;
    for (int i = 0; i < BENCH_N; i++)
        bench_buf[i] = draw_one(ivx);
    for (int i = 0; i < BENCH_N; i++)
        bench_y_[i] = draw_one(ivy);

    volatile float dummy;
    double start = bench_clock();
    for (int rep = 0; rep < BENCH_REPS; rep++)
        for (int i = 0; i < BENCH_N; i++)
            dummy = f(bench_buf[i], bench_y_[i]);
    return bench_clock() - start;
    dummy;
}

int main(void)
{
    BenchInterval iv = BENCH_FULL;

#ifdef BENCH_COREMATH
    double m = bench2_(metallic_powf, iv, iv);
#ifdef BENCH_WASM
    double c = bench2_(cr_powf, iv, iv);
    printf("%-6s %-7s metallic %8.4f  core-math %8.4f  cr/m %5.2f\n",
           BENCH_LABEL, "powf", m, c, c / m);
#else
    double l = bench2_(powf, iv, iv);
    double c = bench2_(cr_powf, iv, iv);
    printf("%-6s %-7s metallic %8.4f  libm %8.4f  core-math %8.4f  cr/m %5.2f\n",
           BENCH_LABEL, "powf", m, l, c, c / m);
#endif
#else
    double m = bench2_(metallic_powf, iv, iv);
    double l = bench2_(powf, iv, iv);
    printf("%-7s metallic %8.4f  libm %8.4f  libm/m %5.2f\n",
           "powf", m, l, l / m);
#endif
}
