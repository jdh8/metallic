#include "src/math/float/exp2m1f.c"
#include "src/math/double/exp2m1.c"
#include "binary32/exp2m1/exp2m1f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return exp2m1f(x); }
static float ref(float x) { return ref_exp2m1(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
