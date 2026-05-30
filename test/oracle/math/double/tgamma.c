#include "src/math/double/tgamma.c"
#include "binary64/tgamma/tgamma_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return tgamma(x); }
static double ref(double x) { return ref_tgamma(x); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/tgamma/tgamma.wc");
    bad |= sample_f64(fut, ref, 0, 7, 0, 20000000);
    bad |= sample_f64(fut, ref, -5, 7, 1, 10000000);
    bad |= sample_f64(fut, ref, -1074, -1, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK\n", stderr);
    return bad != 0;
}
