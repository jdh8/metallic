#include "src/math/float/log1pf.c"
#include "binary32/log1p/log1pf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return log1pf(x); }
static float ref(float x) { return ref_log1p(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
