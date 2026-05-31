#ifndef METALLIC_ORACLE_COREMATH_H
#define METALLIC_ORACLE_COREMATH_H

/* Exhaustive binary32 cross-check against CORE-MATH's correctly-rounded cr_*.
 *
 * Both Metallic and CORE-MATH claim correct rounding, so over all 2^32 binary32
 * inputs the two MUST agree bit-for-bit (NaN payloads aside).  This is a fast
 * proof -- no MPFR per call -- complementary to the MPFR sweep in sweep.h: it
 * pins Metallic to the same reference implementation CORE-MATH itself ships,
 * and any disagreement is a genuine correct-rounding bug in one of them. */

#include "oracle.h"
#include <math.h>
#include <stdio.h>

static int coremath_f32(float fut(float), float cr(float))
{
    enum { LIST_CAP = 64 };
    uint64_t bad = 0;
    int64_t worst = 0;
    uint32_t worst_x = 0;
    uint32_t bad_x[LIST_CAP];
    int listed = 0;

    #pragma omp parallel for schedule(static) reduction(+:bad)
    for (int64_t k = 0; k <= 0xFFFFFFFF; ++k) {
        float x = reinterpret(float, (uint32_t)k);

        float a = fut(x), b = cr(x);

        /* NaN inputs are swept too: a non-NaN result for a NaN input is flagged;
         * the any-NaN-matches-any-NaN clause accepts a correct NaN return. */
        if ((isnan(a) && isnan(b)) || reinterpret(uint32_t, a) == reinterpret(uint32_t, b))
            continue;

        ++bad;

        #pragma omp critical
        {
            int64_t d = ulp_dist_f(a, b);

            if (d < 0)
                d = -d;

            if (d > worst) {
                worst = d;
                worst_x = (uint32_t)k;
            }

            if (listed < LIST_CAP)
                bad_x[listed++] = (uint32_t)k;
        }
    }

    if (bad) {
        fprintf(stderr, "FAIL: %llu disagreements vs CORE-MATH, worst %lld ulp at x=%a; first %d:\n",
            (unsigned long long)bad, (long long)worst,
            (double)reinterpret(float, worst_x), listed);

        for (int i = 0; i < listed; ++i) {
            float x = reinterpret(float, bad_x[i]);
            fprintf(stderr, "  x=%a  metallic=%a  core-math=%a\n",
                (double)x, (double)fut(x), (double)cr(x));
        }

        return 1;
    }

    fputs("OK: bit-identical to CORE-MATH over all 2^32 binary32 inputs\n", stderr);
    return 0;
}

#endif
