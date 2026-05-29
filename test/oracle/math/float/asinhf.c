#include "src/math/float/asinhf.c"
#include "binary32/asinh/asinhf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return asinhf(x); }
static float ref(float x) { return ref_asinh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
