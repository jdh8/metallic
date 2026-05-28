#include "src/math/float/logf.c"
#include "binary32/log/logf_mpfr.c"
#include "../../sweep.h"

static float fut(float x) { return logf(x); }
static float ref(float x) { return ref_log(x); }

int main(void)
{
    return sweep_f32(fut, ref, ref_init);
}
