#include "src/math/double/atan.c"
#include "binary64/atan/atan_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return atan(x); }
static double ref(double x) { return ref_atan(x); }

int main(void)
{
    uint64_t bad = 0;
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/atan/atan.wc");
    bad |= sample_f64(fut, ref, -60, 60, 1, 20000000);
    bad |= sample_f64(fut, ref, 0, 1023, 1, 10000000);
    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
