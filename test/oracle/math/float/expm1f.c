#include "src/math/float/expm1f.c"
#include "binary32/expm1/expm1f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return expm1f(x); }
static float ref(float x) { return ref_expm1(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
