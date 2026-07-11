#include "src/math/float/atanpif.c"
#include "binary32/atanpi/atanpif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return atanpif(x); }
static float ref(float x) { return ref_atanpi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
