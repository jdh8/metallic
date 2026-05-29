#include "src/math/float/erfcf.c"
#include "binary32/erfc/erfcf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return erfcf(x); }
static float ref(float x) { return ref_erfc(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
