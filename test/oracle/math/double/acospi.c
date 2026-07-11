#include "src/math/double/acospi.c"
#include "binary64/acospi/acospi_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return acospi(x); }
static double ref(double x) { return ref_acospi(x); }

/* Dense walk toward the domain edges +-1, where 1 - |x| is the small
 * parameter (random mantissas rarely land there) and the accurate tier
 * must stay cancellation-free. */
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

    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/acospi/acospi.wc");
    bad |= edge_walk();
    bad |= sample_f64(fut, ref, -1023, -890, 1, 5000000);  /* near 1/2 results */
    bad |= sample_f64(fut, ref, -890, -27, 1, 10000000);   /* tiny band */
    bad |= sample_f64(fut, ref, -30, -24, 1, 5000000);     /* 2^-27 seam */
    bad |= sample_f64(fut, ref, -27, -1, 1, 20000000);     /* main band */
    bad |= sample_f64(fut, ref, -3, -1, 1, 10000000);      /* dense in [1/8, 1) */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
