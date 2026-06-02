/* cabsf is hypotf, already proven correctly rounded; this driver validates the
 * complex harness against a known-CR function (a clean run means no false
 * failures) before it is trusted for the genuinely-complex functions. */
#include "src/math/float/cabsf.c"
#include "cmpfr.h"
#include "../../../complex.h"

static float _Complex fut(float _Complex z) { return cabsf(z); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_cf(fut, ref_cabsf, -120, 120, 1, 20000000);
    bad |= sample_cf(fut, ref_cabsf, -8, 8, 1, 10000000);
    bad |= sample_cf_box(fut, ref_cabsf, 60, 120, -120, -60, 1, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_cabsf, -120, 120, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
