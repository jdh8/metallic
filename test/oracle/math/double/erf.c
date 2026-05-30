#include "src/math/double/erf.c"
#include "binary64/erf/erf_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return erf(x); }
static double ref(double x) { return ref_erf(x); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/erf/erf.wc");
    bad |= sample_f64(fut, ref, -6, 6, 1, 20000000);
    bad |= sample_f64(fut, ref, -10, -7, 1, 5000000);
    bad |= sample_f64(fut, ref, 7, 10, 0, 5000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
