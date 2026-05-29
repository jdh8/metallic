#include "src/math/double/asin.c"
#include "binary64/asin/asin_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return asin(x); }
static double ref(double x) { return ref_asin(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/asin/asin.wc");
    bad |= sample_f64(fut, ref, -1023, 0, 1, 20000000);  /* |x| in (0,1], signed */
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
