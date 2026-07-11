#include "src/math/float/acospif.c"
#include "binary32/acospi/acospif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return acospif(x); }
static float ref(float x) { return ref_acospi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
