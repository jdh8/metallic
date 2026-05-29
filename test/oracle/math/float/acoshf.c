#include "src/math/float/acoshf.c"
#include "binary32/acosh/acoshf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return acoshf(x); }
static float ref(float x) { return ref_acosh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
