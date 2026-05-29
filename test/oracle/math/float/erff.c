#include "src/math/float/erff.c"
#include "binary32/erf/erff_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return erff(x); }
static float ref(float x) { return ref_erf(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
