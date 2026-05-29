#include "src/math/float/coshf.c"
#include "binary32/cosh/coshf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return coshf(x); }
static float ref(float x) { return ref_cosh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
