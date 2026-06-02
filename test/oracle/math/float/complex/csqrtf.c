#include "src/math/float/csqrtf.c"
#include "cmpfr.h"
#include "../../../complex.h"

static float _Complex fut(float _Complex z) { return csqrtf(z); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_cf(fut, ref_csqrtf, -120, 120, 1, 20000000);
    bad |= sample_cf(fut, ref_csqrtf, -8, 8, 1, 10000000);
    bad |= sample_cf_box(fut, ref_csqrtf, 60, 120, -120, -60, 1, 10000000);
    /* tiny |z|: drives subnormal outputs through the double-double path */
    bad |= sample_cf(fut, ref_csqrtf, -149, -120, 1, 10000000);
    /* huge |z|: large normal outputs */
    bad |= sample_cf(fut, ref_csqrtf, 100, 127, 1, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_csqrtf, -120, 120, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_csqrtf, -149, -120, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
