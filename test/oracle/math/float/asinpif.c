#include "src/math/float/asinpif.c"
#include "binary32/asinpi/asinpif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return asinpif(x); }
static float ref(float x) { return ref_asinpi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
