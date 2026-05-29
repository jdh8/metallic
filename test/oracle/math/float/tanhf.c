#include "src/math/float/tanhf.c"
#include "binary32/tanh/tanhf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return tanhf(x); }
static float ref(float x) { return ref_tanh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
