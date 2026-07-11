#include "src/math/double/atan2pi.c"
#include "binary64/atan2pi/atan2pi_mpfr.c"
#include "../../sample.h"

static double fut(double y, double x) { return atan2pi(y, x); }
static double ref(double y, double x) { return ref_atan2pi(y, x); }

/* The finite diagonal |y| = |x| must land exactly on +-1/4 and +-3/4. */
static uint64_t diagonal(void)
{
    uint64_t bad = 0;

    for (int i = 0; i < 200000; ++i) {
        double yv = sample_draw_(-1023, 1023, 1);
        double xv = sample_next_() & 1 ? fabs(yv) : -fabs(yv);
        int64_t d = sample_ulp_(fut(yv, xv), ref(yv, xv));

        if (d && ++bad <= 10)
            fprintf(stderr, "  y=%a x=%a  got=%a  want=%a  (%lld ulp)\n",
                    yv, xv, fut(yv, xv), ref(yv, xv), (long long)d);
    }
    fprintf(stderr, "  diagonal |y|=|x| (200000 pairs): %llu bad\n",
            (unsigned long long)bad);
    return bad;
}

int main(void)
{
    uint64_t bad = 0;

    /* Tiny quotients give subnormal results; constrain MPFR's exponent range
     * so the reference's mpfr_subnormalize rounds exactly once. */
    ref_init();
    bad |= sample_wc_f64_2(fut, ref, CORE_MATH "/binary64/atan2pi/atan2pi.wc");
    bad |= diagonal();
    bad |= sample_f64_2(fut, ref, -60, 60, -60, 60, 1, 20000000);
    bad |= sample_f64_2(fut, ref, -5, 5, -5, 5, 1, 10000000);         /* near-diagonal */
    bad |= sample_f64_2(fut, ref, -1023, 1023, -1023, 1023, 1, 10000000);
    bad |= sample_f64_2(fut, ref, -1023, -950, -60, 60, 1, 5000000);  /* tiny quotients */
    bad |= sample_f64_2(fut, ref, -60, 60, -1023, -950, 1, 5000000);  /* + swapped */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
