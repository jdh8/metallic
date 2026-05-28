#include "src/math/float/expf.c"
#include "binary32/exp/expf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return expf(x); }
static float ref(float x) { return ref_exp(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
