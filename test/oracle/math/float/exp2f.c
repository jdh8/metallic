#include "src/math/float/exp2f.c"
#include "binary32/exp2/exp2f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return exp2f(x); }
static float ref(float x) { return ref_exp2(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
