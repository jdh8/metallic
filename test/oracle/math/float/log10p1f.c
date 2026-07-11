#include "src/math/float/log10p1f.c"
#include "binary32/log10p1/log10p1f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return log10p1f(x); }
static float ref(float x) { return ref_log10p1(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
