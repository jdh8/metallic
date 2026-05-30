#include "src/math/double/atanh.c"
#include "binary64/atanh/atanh_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return atanh(x); }
static double ref(double x) { return ref_atanh(x); }

int main(void)
{
    uint64_t bad = 0;
    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/atanh/atanh.wc");
    bad |= sample_f64(fut, ref, -1023, 0, 1, 20000000);  /* |x| in (0,1], signed */
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
