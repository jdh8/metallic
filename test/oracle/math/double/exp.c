#include "src/math/double/exp.c"
#include "binary64/exp/exp_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return exp(x); }
static double ref(double x) { return ref_exp(x); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/exp/exp.wc");
    bad |= sample_f64(fut, ref, -55, 9, 1, 20000000);
    bad |= sample_f64(fut, ref, -3, 9, 1, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
