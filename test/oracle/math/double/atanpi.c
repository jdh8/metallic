#include "src/math/double/atanpi.c"
#include "binary64/atanpi/atanpi_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return atanpi(x); }
static double ref(double x) { return ref_atanpi(x); }

int main(void)
{
    uint64_t bad = 0;

    /* Tiny inputs give subnormal results; constrain MPFR's exponent range so
     * the reference's mpfr_subnormalize rounds exactly once. */
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/atanpi/atanpi.wc");
    bad |= sample_f64(fut, ref, -1023, -890, 1, 5000000);  /* subnormals + deep tail */
    bad |= sample_f64(fut, ref, -910, -890, 1, 5000000);   /* 2^-900 seam */
    bad |= sample_f64(fut, ref, -890, -27, 1, 10000000);   /* Dekker tiny band */
    bad |= sample_f64(fut, ref, -30, -24, 1, 5000000);     /* 2^-27 seam */
    bad |= sample_f64(fut, ref, -27, 2, 1, 20000000);      /* moderate */
    bad |= sample_f64(fut, ref, -60, 60, 1, 10000000);
    bad |= sample_f64(fut, ref, 0, 1023, 1, 10000000);     /* large: +-1/2 approach */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
