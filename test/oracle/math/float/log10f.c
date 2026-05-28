#include "src/math/float/log10f.c"
#include "binary32/log10/log10f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return log10f(x); }
static float ref(float x) { return ref_log10(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
