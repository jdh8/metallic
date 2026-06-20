#include "src/math/double/pow.c"
#include "binary64/pow/pow_mpfr.c"
#include "../../sample.h"

static double fut(double x, double y) { return pow(x, y); }
static double ref(double x, double y) { return ref_pow(x, y); }

int main(void)
{
    uint64_t bad = 0;

    /* ref_pow computes at precision 53 then mpfr_subnormalize; without the
     * binary64 exponent range that idiom DOUBLE-ROUNDS in the subnormal range,
     * reporting spurious 1-ulp misses.  Constrain MPFR to binary64 so the
     * reference is genuinely correctly rounded everywhere. */
    mpfr_set_emin(-1073);
    mpfr_set_emax(1024);

    /* The published hardest-to-round pow(x,y) pairs. */
    bad |= sample_wc_f64_2(fut, ref, CORE_MATH "/binary64/pow/pow.wc");

    /* x near 1 with large |y| is the hard regime (the y amplification of the
     * log error is worst there); then a wide spread. */
    bad |= sample_f64_2(fut, ref, -1, 1, -10, 10, 0, 20000000);
    bad |= sample_f64_2(fut, ref, -20, 20, -6, 6, 0, 20000000);
    bad |= sample_f64_2(fut, ref, -60, 60, -2, 4, 0, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
