#include "src/math/double/acosh.c"
#include "binary64/acosh/acosh_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return acosh(x); }
static double ref(double x) { return ref_acosh(x); }

int main(void)
{
    uint64_t bad = 0;
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/acosh/acosh.wc");
    bad |= sample_f64(fut, ref, 0, 1023, 0, 20000000);  /* x >= 1, unsigned (emin=0 means x>=1) */
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
