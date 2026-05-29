#include "src/math/float/cosf.c"
#include "src/math/float/rem_pio2f.c"
#include "binary32/cos/cosf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return cosf(x); }
static float ref(float x) { return ref_cos(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
