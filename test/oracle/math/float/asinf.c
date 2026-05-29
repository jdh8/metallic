#include "src/math/float/asinf.c"
#include "binary32/asin/asinf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return asinf(x); }
static float ref(float x) { return ref_asin(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
