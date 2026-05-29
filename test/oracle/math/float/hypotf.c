#include "src/math/float/hypotf.c"
#include "binary32/hypot/hypotf_mpfr.c"
#include "../../sample.h"

static float fut(float x, float y) { return hypotf(x, y); }
static float ref(float x, float y) { return ref_hypot(x, y); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f32_2(fut, ref, CORE_MATH "/binary32/hypot/hypotf.wc");
    bad |= sample_f32_2(fut, ref, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, ref, -60, 60, 1, 20000000);
    bad |= sample_f32_2(fut, ref, 100, 127, 1, 20000000);
    bad |= sample_f32_2(fut, ref, -126, -100, 1, 20000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
