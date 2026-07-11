#include "src/math/double/atan2.c"
#include "binary64/atan2/atan2_mpfr.c"
#include "../../sample.h"

static double fut(double y, double x) { return atan2(y, x); }
static double ref(double y, double x) { return ref_atan2(y, x); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f64_2(fut, ref, CORE_MATH "/binary64/atan2/atan2.wc");
    bad |= sample_f64_2(fut, ref, -60, 60, -60, 60, 1, 20000000);
    bad |= sample_f64_2(fut, ref, 0, 1023, 0, 1023, 1, 10000000);
    bad |= sample_f64_2(fut, ref, -1074, -1023, -1074, -1023, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
