#include "src/math/float/atanhf.c"
#include "binary32/atanh/atanhf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return atanhf(x); }
static float ref(float x) { return ref_atanh(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
