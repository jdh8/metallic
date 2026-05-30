#include "src/math/double/tanh.c"
#include "binary64/tanh/tanh_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return tanh(x); }
static double ref(double x) { return ref_tanh(x); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/tanh/tanh.wc");
    bad |= sample_f64(fut, ref, -9, 9, 1, 20000000);
    bad |= sample_f64(fut, ref, -1022, -9, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
