#include "src/math/float/acosf.c"
#include "binary32/acos/acosf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return acosf(x); }
static float ref(float x) { return ref_acos(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
