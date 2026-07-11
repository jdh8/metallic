#include "src/math/float/sinpif.c"
#include "binary32/sinpi/sinpif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return sinpif(x); }
static float ref(float x) { return ref_sinpi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
