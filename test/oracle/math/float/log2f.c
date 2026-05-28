#include "src/math/float/log2f.c"
#include "binary32/log2/log2f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return log2f(x); }
static float ref(float x) { return ref_log2(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
