#include "src/math/float/rsqrtf.c"
#include "binary32/rsqrt/rsqrtf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return rsqrtf(x); }
static float ref(float x) { return ref_rsqrt(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
