#include "src/math/double/asinpi.c"
#include "binary64/asinpi/asinpi_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return asinpi(x); }
static double ref(double x) { return ref_asinpi(x); }

/* Dense walk toward the domain edges +-1, where 1 - |x| is the small
 * parameter (random mantissas rarely land there). */
static uint64_t edge_walk(void)
{
    uint64_t bad = 0;

    for (int s = 0; s < 2; ++s) {
        double x = s ? -1.0 : 1.0;

        for (int i = 0; i < 20000; ++i) {
            x = nextafter(x, 0.0);
            int64_t d = sample_ulp_(fut(x), ref(x));

            if (d && ++bad <= 10)
                fprintf(stderr, "  x=%a  got=%a  want=%a  (%lld ulp)\n",
                        x, fut(x), ref(x), (long long)d);
        }
    }
    fprintf(stderr, "  edge walk near +-1 (40000 inputs): %llu bad\n",
            (unsigned long long)bad);
    return bad;
}

int main(void)
{
    uint64_t bad = 0;

    /* Tiny inputs give subnormal results; constrain MPFR's exponent range so
     * the reference's mpfr_subnormalize rounds exactly once. */
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/asinpi/asinpi.wc");
    bad |= edge_walk();
    bad |= sample_f64(fut, ref, -1023, -890, 1, 5000000);  /* subnormals + deep tail */
    bad |= sample_f64(fut, ref, -910, -890, 1, 5000000);   /* 2^-900 seam */
    bad |= sample_f64(fut, ref, -890, -27, 1, 10000000);   /* Dekker tiny band */
    bad |= sample_f64(fut, ref, -30, -24, 1, 5000000);     /* 2^-27 seam */
    bad |= sample_f64(fut, ref, -27, -1, 1, 20000000);     /* main band */
    bad |= sample_f64(fut, ref, -3, -1, 1, 10000000);      /* dense in [1/8, 1) */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
