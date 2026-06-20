#include "src/math/double/pow.c"
#include "binary64/pow/pow_mpfr.c"
#include "../../sample.h"

static double fut(double x, double y) { return pow(x, y); }
static double ref(double x, double y) { return ref_pow(x, y); }

int main(void)
{
    uint64_t bad = 0;

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
