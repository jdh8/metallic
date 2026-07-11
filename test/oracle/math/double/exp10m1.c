#include "src/math/double/exp10m1.c"
#include "binary64/exp10m1/exp10m1_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return exp10m1(x); }
static double ref(double x) { return ref_exp10m1(x); }

int main(void)
{
    uint64_t bad = 0;

    /* Reduce MPFR's exponent range so mpfr_subnormalize in the reference is
     * sound for the subnormal results this test samples. */
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/exp10m1/exp10m1.wc");
    bad |= sample_f64(fut, ref, -60, 9, 1, 20000000);    /* both bands */
    bad |= sample_f64(fut, ref, -5, 9, 1, 10000000);     /* main band */
    bad |= sample_f64(fut, ref, -1023, -890, 1, 5000000); /* deep dint tier */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
