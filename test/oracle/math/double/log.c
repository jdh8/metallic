#include "src/math/double/log.c"
#include "binary64/log/log_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return log(x); }
static double ref(double x) { return ref_log(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/log/log.wc");
    bad |= sample_f64(fut, ref, -55, 10, 0, 20000000);
    bad |= sample_f64(fut, ref, -3, 10, 0, 10000000);
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
