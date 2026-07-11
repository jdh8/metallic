#include "src/math/float/tanpif.c"
#include "binary32/tanpi/tanpif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return tanpif(x); }
static float ref(float x) { return ref_tanpi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
