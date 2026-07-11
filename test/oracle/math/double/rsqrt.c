#include "src/math/double/rsqrt.c"
#include "binary64/rsqrt/rsqrt_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return rsqrt(x); }
static double ref(double x) { return ref_rsqrt(x); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/rsqrt/rsqrt.wc");
    /* Full domain: subnormals (e = -1023) through the top binade. */
    bad |= sample_f64(fut, ref, -1023, 1023, 0, 20000000);
    bad |= sample_f64(fut, ref, -1023, -1000, 0, 5000000);
    /* Signed draws exercise the NaN domain arm. */
    bad |= sample_f64(fut, ref, -60, 60, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
