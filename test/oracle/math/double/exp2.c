#include "src/math/double/exp2.c"
#include "binary64/exp2/exp2_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return exp2(x); }
static double ref(double x) { return ref_exp2(x); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/exp2/exp2.wc");
    bad |= sample_f64(fut, ref, -55, 10, 1, 20000000);
    bad |= sample_f64(fut, ref, -3, 10, 1, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
