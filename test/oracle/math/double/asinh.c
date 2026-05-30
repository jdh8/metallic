#include "src/math/double/asinh.c"
#include "binary64/asinh/asinh_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return asinh(x); }
static double ref(double x) { return ref_asinh(x); }

int main(void)
{
    uint64_t bad = 0;
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/asinh/asinh.wc");
    bad |= sample_f64(fut, ref, -1023, 1023, 1, 20000000);  /* all magnitudes, signed */
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
