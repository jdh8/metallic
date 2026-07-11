#include "src/math/float/exp10f.c"
#include "binary32/exp10/exp10f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return exp10f(x); }
static float ref(float x) { return ref_exp10(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
