#include "src/math/float/cospif.c"
#include "binary32/cospi/cospif_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return cospif(x); }
static float ref(float x) { return ref_cospi(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
