#include "src/math/float/atan2f.c"
#include "binary32/atan2/atan2f_mpfr.c"
#include "../../sample.h"

static float fut(float y, float x) { return atan2f(y, x); }
static float ref(float y, float x) { return ref_atan2(y, x); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f32_2(fut, ref, CORE_MATH "/binary32/atan2/atan2f.wc");
    bad |= sample_f32_2(fut, ref, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, ref, -60, 60, 1, 20000000);
    bad |= sample_f32_2(fut, ref, 100, 127, 1, 20000000);
    bad |= sample_f32_2(fut, ref, -126, -100, 1, 20000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
