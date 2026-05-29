#include "src/math/float/atanf.c"
#include "binary32/atan/atanf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return atanf(x); }
static float ref(float x) { return ref_atan(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
