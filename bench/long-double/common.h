#ifndef METALLIC_BENCH_LONG_DOUBLE_COMMON_H
#define METALLIC_BENCH_LONG_DOUBLE_COMMON_H

#include "src/math/long-double/kernel/binary128.h"
#include <stdio.h>
#include <time.h>

typedef union {
    u128 bits;
    long double metallic;
    __float128 core_math;
} bench_value_;

typedef struct {
    int low_exponent;
    int high_exponent;
    unsigned mode;
} bench_interval_;

#define BENCH_NORMAL ((bench_interval_){0, 0, 0})
#define BENCH_POSITIVE_NORMAL ((bench_interval_){0, 0, 1})
#define BENCH_EXPONENTS(low, high) ((bench_interval_){(low), (high), 2})

/* Sixteen million calls keep even the fastest software binary128 paths above
 * scheduler-scale noise without approaching the cost of the binary32 sweep. */
enum { BENCH_N_ = 16384, BENCH_REPS_ = 1024, BENCH_BATCHES_ = 4 };
_Static_assert(BENCH_REPS_ % BENCH_BATCHES_ == 0,
    "benchmark batches must divide the repetition count");

static bench_value_ bench_x_[BENCH_N_];
static bench_value_ bench_y_[BENCH_N_];
static uint64_t bench_rng_state_;

static uint64_t bench_random64_(void)
{
    uint64_t value = (bench_rng_state_ += UINT64_C(0x9E3779B97F4A7C15));
    value = (value ^ (value >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    value = (value ^ (value >> 27)) * UINT64_C(0x94D049BB133111EB);
    return value ^ (value >> 31);
}

static u128 bench_random128_(void)
{
    u128 high = bench_random64_();
    u128 low = bench_random64_();
    return (high << 64) | low;
}

static bench_value_ bench_draw_(bench_interval_ interval)
{
    bench_value_ value;
    value.bits = bench_random128_();

    if (interval.mode < 2) {
        u128 exponent = (value.bits >> F128_EXP_SHIFT) & 0x7fff;
        while (!exponent || exponent == 0x7fff) {
            value.bits = bench_random128_();
            exponent = (value.bits >> F128_EXP_SHIFT) & 0x7fff;
        }
        if (interval.mode == 1)
            value.bits &= ~F128_SIGN_MASK;
        return value;
    }

    unsigned width = (unsigned)(interval.high_exponent - interval.low_exponent + 1);
    int exponent = interval.low_exponent + (int)(bench_random64_() % width);
    value.bits = (value.bits & (F128_SIGN_MASK | F128_MANTISSA_MASK))
        | (u128)(exponent + F128_BIAS) << F128_EXP_SHIFT;
    return value;
}

static __attribute__((unused)) void bench_prepare_unary_(bench_interval_ interval)
{
    bench_rng_state_ = UINT64_C(0x243F6A8885A308D3);
    for (unsigned i = 0; i < BENCH_N_; ++i)
        bench_x_[i] = bench_draw_(interval);
}

static __attribute__((unused)) void
bench_prepare_binary_(bench_interval_ x, bench_interval_ y)
{
    bench_rng_state_ = UINT64_C(0x243F6A8885A308D3);
    for (unsigned i = 0; i < BENCH_N_; ++i) {
        bench_x_[i] = bench_draw_(x);
        bench_y_[i] = bench_draw_(y);
    }
}

typedef long double bench_metallic_unary_(long double);
typedef long double bench_metallic_binary_(long double, long double);
typedef __float128 bench_core_unary_(__float128);
typedef __float128 bench_core_binary_(__float128, __float128);

static __attribute__((noinline, unused)) double
bench_metallic_unary_time_(bench_metallic_unary_ *function, unsigned repeats)
{
    volatile long double result = function(bench_x_[0].metallic);
    clock_t start = clock();
    for (unsigned repeat = 0; repeat < repeats; ++repeat)
        for (unsigned i = 0; i < BENCH_N_; ++i)
            result = function(bench_x_[i].metallic);
    (void)result;
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static __attribute__((noinline, unused)) double
bench_core_unary_time_(bench_core_unary_ *function, unsigned repeats)
{
    volatile __float128 result = function(bench_x_[0].core_math);
    clock_t start = clock();
    for (unsigned repeat = 0; repeat < repeats; ++repeat)
        for (unsigned i = 0; i < BENCH_N_; ++i)
            result = function(bench_x_[i].core_math);
    (void)result;
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static __attribute__((noinline, unused)) double
bench_metallic_binary_time_(bench_metallic_binary_ *function, unsigned repeats)
{
    volatile long double result = function(bench_x_[0].metallic,
        bench_y_[0].metallic);
    clock_t start = clock();
    for (unsigned repeat = 0; repeat < repeats; ++repeat)
        for (unsigned i = 0; i < BENCH_N_; ++i)
            result = function(bench_x_[i].metallic, bench_y_[i].metallic);
    (void)result;
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static __attribute__((noinline, unused)) double
bench_core_binary_time_(bench_core_binary_ *function, unsigned repeats)
{
    volatile __float128 result = function(bench_x_[0].core_math,
        bench_y_[0].core_math);
    clock_t start = clock();
    for (unsigned repeat = 0; repeat < repeats; ++repeat)
        for (unsigned i = 0; i < BENCH_N_; ++i)
            result = function(bench_x_[i].core_math, bench_y_[i].core_math);
    (void)result;
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static __attribute__((unused)) void bench_unary_times_(
    bench_metallic_unary_ *metallic_function, bench_core_unary_ *core_function,
    double *metallic, double *core_math)
{
    unsigned repeats = BENCH_REPS_ / BENCH_BATCHES_;
    *metallic = *core_math = 0;

    for (unsigned batch = 0; batch < BENCH_BATCHES_; ++batch) {
        if (batch & 1) {
            *core_math += bench_core_unary_time_(core_function, repeats);
            *metallic += bench_metallic_unary_time_(metallic_function, repeats);
        } else {
            *metallic += bench_metallic_unary_time_(metallic_function, repeats);
            *core_math += bench_core_unary_time_(core_function, repeats);
        }
    }
}

static __attribute__((unused)) void bench_binary_times_(
    bench_metallic_binary_ *metallic_function, bench_core_binary_ *core_function,
    double *metallic, double *core_math)
{
    unsigned repeats = BENCH_REPS_ / BENCH_BATCHES_;
    *metallic = *core_math = 0;

    for (unsigned batch = 0; batch < BENCH_BATCHES_; ++batch) {
        if (batch & 1) {
            *core_math += bench_core_binary_time_(core_function, repeats);
            *metallic += bench_metallic_binary_time_(metallic_function, repeats);
        } else {
            *metallic += bench_metallic_binary_time_(metallic_function, repeats);
            *core_math += bench_core_binary_time_(core_function, repeats);
        }
    }
}

#ifndef BENCH_LABEL
#define BENCH_LABEL "quad"
#endif

#define BENCH_QUAD_UNARY(function, core, interval) int main(void) {                 \
    bench_prepare_unary_(interval);                                                \
    double metallic, core_math;                                                    \
    bench_unary_times_(function, core, &metallic, &core_math);                     \
    printf("%-5s %-7s metallic %8.4f  core-math %8.4f  cr/m %5.2f\n",            \
        BENCH_LABEL, #function, metallic, core_math, core_math / metallic);         \
}

#define BENCH_QUAD_BINARY(function, core, x_interval, y_interval) int main(void) {  \
    bench_prepare_binary_(x_interval, y_interval);                                 \
    double metallic, core_math;                                                    \
    bench_binary_times_(function, core, &metallic, &core_math);                    \
    printf("%-5s %-7s metallic %8.4f  core-math %8.4f  cr/m %5.2f\n",            \
        BENCH_LABEL, #function, metallic, core_math, core_math / metallic);         \
}

#endif
