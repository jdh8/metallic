#include "src/math/float/log2p1f.c"
#include "binary32/log2p1/log2p1f_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return log2p1f(x); }
static float ref(float x) { return ref_log2p1(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
