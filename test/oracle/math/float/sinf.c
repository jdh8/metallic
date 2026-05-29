#include "src/math/float/sinf.c"
#include "src/math/float/rem_pio2f.c"
#include "binary32/sin/sinf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return sinf(x); }
static float ref(float x) { return ref_sin(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
