#include "src/math/float/cbrtf.c"
#include "binary32/cbrt/cbrtf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return cbrtf(x); }
static float ref(float x) { return ref_cbrt(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
