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
    uint64_t bad = 0;
    int64_t worst = 0;
    uint32_t worst_x = 0, first_x = 0;
    int have_first = 0;

    #pragma omp parallel
    {
        init();

        #pragma omp for schedule(static) reduction(+:bad)
        for (int64_t k = 0; k <= 0xFFFFFFFF; ++k) {
            float x = reinterpret(float, (uint32_t)k);

            if (isnan(x))
                continue;

            if (reinterpret(uint32_t, fut(x)) == reinterpret(uint32_t, ref(x)))
                continue;

            ++bad;

            #pragma omp critical
            {
                int64_t d = ulp_dist_f(fut(x), ref(x));

                if (!have_first) {
                    have_first = 1;
                    first_x = (uint32_t)k;
                }

                if (d < 0)
                    d = -d;

                if (d > worst) {
                    worst = d;
                    worst_x = (uint32_t)k;
                }
            }
        }
    }

    if (bad) {
        float x = reinterpret(float, first_x);
        float w = reinterpret(float, worst_x);

        fprintf(stderr,
            "FAIL: %llu mismatches; first x=%a -> got %a want %a; "
            "worst %lld ulp at x=%a (got %a want %a)\n",
            (unsigned long long)bad, (double)x, (double)fut(x), (double)ref(x),
            (long long)worst, (double)w, (double)fut(w), (double)ref(w));

        return 1;
    }

    fputs("OK: correctly rounded over all 2^32 binary32 inputs\n", stderr);
    return 0;
}

#endif
