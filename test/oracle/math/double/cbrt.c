#include "src/math/double/cbrt.c"
#include "binary64/cbrt/cbrt_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return cbrt(x); }
static double ref(double x) { return ref_cbrt(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/cbrt/cbrt.wc");
    bad |= sample_f64(fut, ref, -300, 300, 1, 20000000);   /* signed, wide exponent */
    bad |= sample_f64(fut, ref, -1074, -1000, 1, 10000000); /* subnormal-ish */
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
