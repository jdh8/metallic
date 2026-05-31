#ifndef METALLIC_ORACLE_SWEEP_H
#define METALLIC_ORACLE_SWEEP_H

/* Exhaustive binary32 verification.
 *
 * Every one of the 2^32 bit patterns is a representable float, so iterating all
 * of them and comparing to a correctly-rounded oracle is a PROOF of correct
 * rounding, not a sample.  Parallelised with OpenMP (minutes on a many-core
 * host).  The oracle (`ref`) allocates per call, so it must be thread-safe for
 * concurrent independent calls -- CORE-MATH's `ref_*` are (local mpfr_t). */

#include "oracle.h"
#include <math.h>
#include <stdio.h>

/* `init` is the oracle's per-thread setup (e.g. CORE-MATH's `ref_init`, which
 * sets MPFR's exponent range).  MPFR's exponent range and constant caches are
 * THREAD-LOCAL in a thread-safe build, so this must run on every worker thread,
 * not once in main -- otherwise subnormal results are misrounded by the oracle
 * on the worker threads only. */
static int sweep_f32(float fut(float), float ref(float), void init(void))
{
    enum { LIST_CAP = 64 };
    uint64_t bad = 0;
    int64_t worst = 0;
    uint32_t worst_x = 0;
    uint32_t bad_x[LIST_CAP];
    int listed = 0;

    #pragma omp parallel
    {
        init();

        #pragma omp for schedule(static) reduction(+:bad)
        for (int64_t k = 0; k <= 0xFFFFFFFF; ++k) {
            float x = reinterpret(float, (uint32_t)k);

            float a = fut(x), b = ref(x);

            /* NaN inputs are swept too: a correctly-rounded function must return
             * NaN, which the any-NaN-matches-any-NaN clause accepts; a non-NaN
             * result for a NaN input is then flagged.  Otherwise require
             * bit-identity. */
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

                /* Collect the first LIST_CAP mismatches so hard-to-round cases
                 * can be read off and patched in one sweep. */
                if (listed < LIST_CAP)
                    bad_x[listed++] = (uint32_t)k;
            }
        }
    }

    if (bad) {
        fprintf(stderr, "FAIL: %llu mismatches, worst %lld ulp at x=%a; first %d:\n",
            (unsigned long long)bad, (long long)worst,
            (double)reinterpret(float, worst_x), listed);

        for (int i = 0; i < listed; ++i) {
            float x = reinterpret(float, bad_x[i]);
            fprintf(stderr, "  x=%a  got=%a  want=%a\n",
                (double)x, (double)fut(x), (double)ref(x));
        }

        return 1;
    }

    fputs("OK: correctly rounded over all 2^32 binary32 inputs\n", stderr);
    return 0;
}

#endif
