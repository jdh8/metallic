#include "src/math/double/tanpi.c"
#include "binary64/tanpi/tanpi_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return tanpi(x); }
static double ref(double x) { return ref_tanpi(x); }

int main(void)
{
    uint64_t bad = 0;

    /* Reduce MPFR's exponent range so mpfr_subnormalize in the reference is
     * sound for the subnormal results this test samples. */
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/tanpi/tanpi.wc");
    bad |= sample_f64(fut, ref, -60, 10, 1, 20000000);    /* small + main bands */
    bad |= sample_f64(fut, ref, -12, 46, 1, 10000000);    /* main band */
    bad |= sample_f64(fut, ref, 46, 53, 1, 5000000);      /* exact 1/64 grid */
    bad |= sample_f64(fut, ref, 53, 1023, 1, 5000000);    /* all even integers */
    bad |= sample_f64(fut, ref, -200, -40, 1, 5000000);   /* pi*x rounding band */
    bad |= sample_f64(fut, ref, -930, -880, 1, 5000000);  /* 2^-916 scale seam */
    bad |= sample_f64(fut, ref, -1023, -890, 1, 5000000); /* subnormal fold */

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
