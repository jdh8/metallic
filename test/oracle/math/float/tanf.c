#include "src/math/float/tanf.c"
#include "src/math/float/rem_pio2f.c"
#include "binary32/tan/tanf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return tanf(x); }
static float ref(float x) { return ref_tan(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
