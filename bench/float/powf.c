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
static float (*volatile bench2_f_[3])(float, float);

static double bench2_time_(int which)
{
    float (*f)(float, float) = bench2_f_[which];
    volatile float dummy;
    double start = bench_clock();
    for (int rep = 0; rep < BENCH_REPS / BENCH_BATCHES; rep++)
        for (int i = 0; i < BENCH_N; i++)
            dummy = f(bench_buf[i], bench_y_[i]);
    double elapsed = bench_clock() - start;
    (void)dummy;
    return elapsed;
}

int main(void)
{
    double t[3];
    bench_fill(bench_buf, BENCH_FULL);
    bench_fill(bench_y_, BENCH_FULL);
    bench2_f_[0] = metallic_powf;

#ifdef BENCH_COREMATH
#ifdef BENCH_WASM
    bench2_f_[1] = cr_powf;
    bench_run(bench2_time_, 2, t);
    printf("%-6s %-7s metallic %8.4f  core-math %8.4f  cr/m %5.2f\n",
           BENCH_LABEL, "powf", t[0], t[1], t[1] / t[0]);
#else
    bench2_f_[1] = powf;
    bench2_f_[2] = cr_powf;
    bench_run(bench2_time_, 3, t);
    printf("%-6s %-7s metallic %8.4f  libm %8.4f  core-math %8.4f  cr/m %5.2f\n",
           BENCH_LABEL, "powf", t[0], t[1], t[2], t[2] / t[0]);
#endif
#else
    bench2_f_[1] = powf;
    bench_run(bench2_time_, 2, t);
    printf("%-7s metallic %8.4f  libm %8.4f  libm/m %5.2f\n",
           "powf", t[0], t[1], t[1] / t[0]);
#endif
}
