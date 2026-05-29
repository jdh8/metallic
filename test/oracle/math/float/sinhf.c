#include "src/math/float/sinhf.c"
#include "binary32/sinh/sinhf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return sinhf(x); }
static float ref(float x) { return ref_sinh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
