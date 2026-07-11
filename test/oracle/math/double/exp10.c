#include "src/math/double/exp10.c"
#include "binary64/exp10/exp10_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return exp10(x); }
static double ref(double x) { return ref_exp10(x); }

int main(void)
{
    uint64_t bad = 0;

    /* Reduce MPFR's exponent range so mpfr_subnormalize in the reference is
     * sound for subnormal results (x in (-323.6, -307.6)). */
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/exp10/exp10.wc");
    bad |= sample_f64(fut, ref, -60, 9, 1, 20000000);
    bad |= sample_f64(fut, ref, -3, 9, 1, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
