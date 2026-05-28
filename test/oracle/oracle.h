#ifndef METALLIC_ORACLE_H
#define METALLIC_ORACLE_H

/* Shared helpers for the correct-rounding oracle tests.
 *
 * These tests run NATIVELY and compare a math function against CORE-MATH's
 * correctly-rounded MPFR reference (`ref_*`, from the vendored `{func}_mpfr.c`).
 * Pass/fail is EXACT bit-equality: a clean run means the function is correctly
 * rounded (<= 0.5 ulp) for the inputs covered.  The ulp-distance helpers below
 * exist only for diagnostics on a miss.
 *
 * The harness must mirror SHIPPED WASM semantics; the Makefile builds it with
 * -ffp-contract=off -mno-fma (no auto-FMA) and without -march=native.  WASM is
 * round-to-nearest only, matching the x86-64 SSE default. */

#include "src/math/reinterpret.h"
#include <stdint.h>

/* Signed distance in ulps between two binary32 values via the IEEE-754
 * total-ordering map.  Inputs assumed non-NaN. */
static inline int64_t ulp_dist_f(float a, float b)
{
    uint32_t ua = reinterpret(uint32_t, a);
    uint32_t ub = reinterpret(uint32_t, b);
    uint32_t ma = ua & 0x80000000u ? ~ua : ua | 0x80000000u;
    uint32_t mb = ub & 0x80000000u ? ~ub : ub | 0x80000000u;

    return (int64_t)ma - (int64_t)mb;
}

/* Signed distance in ulps between two binary64 values.  Inputs assumed
 * non-NaN. */
static inline int64_t ulp_dist_d(double a, double b)
{
    uint64_t ua = reinterpret(uint64_t, a);
    uint64_t ub = reinterpret(uint64_t, b);
    uint64_t ma = ua & 0x8000000000000000u ? ~ua : ua | 0x8000000000000000u;
    uint64_t mb = ub & 0x8000000000000000u ? ~ub : ub | 0x8000000000000000u;

    return (int64_t)(ma - mb);
}

#endif
