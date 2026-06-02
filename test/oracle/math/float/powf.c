#include "src/math/float/powf.c"
#include "binary32/pow/powf_mpfr.c"
#include "../../sample.h"

static float fut(float x, float y) { return powf(x, y); }
static float ref(float x, float y) { return ref_pow(x, y); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f32_2(fut, ref, CORE_MATH "/binary32/pow/powf.wc");
    bad |= sample_f32_2(fut, ref, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, ref, -5, 5, 0, 20000000);
    bad |= sample_f32_2(fut, ref, 100, 127, 0, 10000000);
    bad |= sample_f32_2(fut, ref, -126, -100, 0, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
