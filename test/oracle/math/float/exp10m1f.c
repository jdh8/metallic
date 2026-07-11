#include "src/math/float/exp10m1f.c"
#include "src/math/double/exp10m1.c"
#include "binary32/exp10m1/exp10m1f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return exp10m1f(x); }
static float ref(float x) { return ref_exp10m1(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
