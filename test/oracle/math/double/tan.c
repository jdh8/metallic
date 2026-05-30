#include "src/math/double/rem_pio2.c"
#include "src/math/double/tan.c"
#include "binary64/tan/tan_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return tan(x); }
static double ref(double x) { return ref_tan(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/tan/tan.wc");
    bad |= sample_f64(fut, ref, -10, 10, 1, 20000000);
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
