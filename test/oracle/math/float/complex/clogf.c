#include "src/math/float/clogf.c"
#include "cmpfr.h"
#include "../../../complex.h"

static float _Complex fut(float _Complex z) { return clogf(z); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_cf(fut, ref_clogf, -120, 120, 1, 20000000);
    bad |= sample_cf(fut, ref_clogf, -8, 8, 1, 10000000);
    /* |z| ~ 1: the hard region for the real part (log near 0). */
    bad |= sample_cf(fut, ref_clogf, -2, 1, 1, 20000000);
    bad |= sample_cf_box(fut, ref_clogf, 60, 120, -120, -60, 1, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_clogf, -120, 120, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_clogf, -2, 1, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
