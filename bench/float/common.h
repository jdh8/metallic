#include "src/math/reinterpret.h"
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <fenv.h>

/* The vendored CORE-MATH sources raise FP exceptions on special inputs via
 * feraiseexcept(FE_*).  metallic intentionally omits these macros: it supports
 * no FP exceptions, so FE_ALL_EXCEPT is 0 and the fe* calls are no-ops (C11
 * 7.6p7 -- define an exception macro iff the implementation supports it).
 * Define them locally so CORE-MATH compiles for wasm; the values never matter
 * because feraiseexcept is a no-op.  On native, <fenv.h> already defines them
 * and these guards are skipped. */
#ifndef FE_INVALID
#define FE_INVALID   0x01
#endif
#ifndef FE_DIVBYZERO
#define FE_DIVBYZERO 0x04
#endif
#ifndef FE_OVERFLOW
#define FE_OVERFLOW  0x08
#endif
#ifndef FE_UNDERFLOW
#define FE_UNDERFLOW 0x10
#endif
#ifndef FE_INEXACT
#define FE_INEXACT   0x20
#endif

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

/* Seconds since an arbitrary epoch.  Prefers CPU time (load-independent); falls
 * back to wall time if clock() is unavailable on the runtime.  The bench is
 * single-threaded and CPU-bound, so wall ~= cpu.  On native, clock() never
 * returns -1 so the fallback is dead code; it only matters for a WASI runtime
 * that does not back CLOCK_PROCESS_CPUTIME_ID. */
static double bench_clock(void) {
    clock_t c = clock();
    if (c != (clock_t)-1) return (double)c / CLOCKS_PER_SEC;
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC)) return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
    return 0;
}

static double bench(float f(float), BenchInterval iv) {
    rng_state = (uint32_t)time(NULL) + bench_call++;
    if (!rng_state) rng_state = 1;
    for (int i = 0; i < BENCH_N; i++)
        bench_buf[i] = draw_one(iv);

    volatile float dummy;
    double start = bench_clock();
    for (int rep = 0; rep < BENCH_REPS; rep++)
        for (int i = 0; i < BENCH_N; i++)
            dummy = f(bench_buf[i]);
    return bench_clock() - start;
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
 * (>1 => Metallic faster).
 *
 * On wasm (-DBENCH_WASM) the libm column is dropped: there is no separate wasm
 * libm (plain `f` would resolve to metallic's own symbol from metallic.a), and
 * LLVM inlines that standard-named call into the timed loop while it keeps the
 * non-standard metallic_##f / cr_##f as real calls -- so a libm column would be
 * a misleading inlined artifact, not a fair third measurement.  metallic_##f
 * and cr_##f are both #included into this TU and measured as real calls, so
 * cr/m is apples-to-apples.  See bench/README.md. */
#ifdef BENCH_WASM
#define BENCH_CR(f, iv) int main(void) {                                               \
    double m = bench(metallic_##f, iv), c = bench(cr_##f, iv);                        \
    printf("%-6s %-7s metallic %8.4f  core-math %8.4f  cr/m %5.2f\n",                \
           BENCH_LABEL, #f, m, c, c / m);                                             \
}
#else
#define BENCH_CR(f, iv) int main(void) {                                               \
    double m = bench(metallic_##f, iv), l = bench(f, iv), c = bench(cr_##f, iv);      \
    printf("%-6s %-7s metallic %8.4f  libm %8.4f  core-math %8.4f  cr/m %5.2f\n",   \
           BENCH_LABEL, #f, m, l, c, c / m);                                          \
}
#endif
