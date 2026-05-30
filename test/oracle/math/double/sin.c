#include "src/math/double/rem_pio2.c"
#include "src/math/double/sin.c"
#include "binary64/sin/sin_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return sin(x); }
static double ref(double x) { return ref_sin(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/sin/sin.wc");
    bad |= sample_f64(fut, ref, -10, 10, 1, 20000000);
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
