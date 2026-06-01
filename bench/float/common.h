#include "src/math/reinterpret.h"
#include <math.h>
#include <time.h>
#include <stdint.h>

/* Sampling interval — mirrors the Draw trait in metallic-rs bench.rs.
 * The *form* of the range determines the distribution:
 *   BENCH_FULL:            representation-uniform over all 2^32 bit patterns
 *   BENCH_FROM(lo):        representation-uniform over [lo, +∞]
 *   BENCH_TO(hi):          representation-uniform over [-∞, hi]
 *   BENCH_BOUNDED(lo, hi): value-uniform over [lo, hi] */
typedef struct { float lo, hi; int mode; } BenchInterval;

#define BENCH_FULL            ((BenchInterval){0, 0, 0})
#define BENCH_FROM(lo)        ((BenchInterval){(lo), 0, 1})
#define BENCH_TO(hi)          ((BenchInterval){0, (hi), 2})
#define BENCH_BOUNDED(lo, hi) ((BenchInterval){(lo), (hi), 3})

/* xorshift32 PRNG — seeded fresh at the start of each bench() call. */
static uint32_t rng_state;
static uint32_t bench_call;

static uint32_t rng_next(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

/* Sign-magnitude bijection: maps floats to a monotone uint32_t so that a
 * contiguous integer range corresponds to a contiguous float range.  Matches
 * to_ordered/from_ordered in metallic-rs bench.rs. */
static uint32_t f32_to_ordered(float x) {
    uint32_t b = reinterpret(uint32_t, x);
    return b ^ ((uint32_t)((int32_t)b >> 31) | 0x80000000u);
}

static float f32_from_ordered(uint32_t k) {
    uint32_t b = k ^ ((uint32_t)((int32_t)(k >> 31) - 1) | 0x80000000u);
    return reinterpret(float, b);
}

/* f32_to_ordered of ±inf; NaNs fall above BENCH_ORDERED_INF. */
#define BENCH_ORDERED_INF  0xFF800000u
#define BENCH_ORDERED_NINF 0x007FFFFFu

static float draw_one(BenchInterval iv) {
    switch (iv.mode) {
    case 1: { /* repr-uniform [lo, +inf] */
        uint32_t lo = f32_to_ordered(iv.lo);
        uint32_t span = BENCH_ORDERED_INF - lo;
        return f32_from_ordered(lo + (uint32_t)((uint64_t)rng_next() * (span + 1) >> 32));
    }
    case 2: { /* repr-uniform [-inf, hi] */
        uint32_t lo = BENCH_ORDERED_NINF;
        uint32_t span = f32_to_ordered(iv.hi) - lo;
        return f32_from_ordered(lo + (uint32_t)((uint64_t)rng_next() * (span + 1) >> 32));
    }
    case 3: /* value-uniform [lo, hi] */
        return iv.lo + (iv.hi - iv.lo) * (rng_next() * 0x1p-32f);
    default: /* BENCH_FULL: all 2^32 bit patterns equally likely */
        return f32_from_ordered(rng_next());
    }
}

enum { BENCH_N = 65536, BENCH_REPS = 3440 };
static float bench_buf[BENCH_N];

static double bench(float f(float), BenchInterval iv) {
    rng_state = (uint32_t)time(NULL) + bench_call++;
    if (!rng_state) rng_state = 1;
    for (int i = 0; i < BENCH_N; i++)
        bench_buf[i] = draw_one(iv);

    volatile float dummy;
    clock_t start = clock();
    for (int rep = 0; rep < BENCH_REPS; rep++)
        for (int i = 0; i < BENCH_N; i++)
            dummy = f(bench_buf[i]);
    return (double)(clock() - start) / CLOCKS_PER_SEC;
    dummy;
}

/* Metallic vs system libm.  One row: function, the two sweep times (seconds),
 * and the libm/metallic ratio (>1 => Metallic faster).  See bench/README.md. */
#define BENCH(f, iv) int main(void) {                              \
    double m = bench(metallic_##f, iv), l = bench(f, iv);         \
    printf("%-7s metallic %8.4f  libm %8.4f  libm/m %5.2f\n",    \
           #f, m, l, l / m);                                       \
}

#ifndef BENCH_LABEL
#define BENCH_LABEL "?"
#endif

/* Metallic vs system libm vs CORE-MATH (cr_*).  One self-describing row: model
 * label, function, the three sweep times (s), and the cr/metallic ratio
 * (>1 => Metallic faster). */
#define BENCH_CR(f, iv) int main(void) {                                               \
    double m = bench(metallic_##f, iv), l = bench(f, iv), c = bench(cr_##f, iv);      \
    printf("%-6s %-7s metallic %8.4f  libm %8.4f  core-math %8.4f  cr/m %5.2f\n",   \
           BENCH_LABEL, #f, m, l, c, c / m);                                          \
}
