/* cargf is atan2f, already proven correctly rounded; this driver validates the
 * complex harness against a known-CR function before it is trusted for the
 * genuinely-complex functions. */
#include "src/math/float/cargf.c"
#include "cmpfr.h"
#include "../../../complex.h"

static float _Complex fut(float _Complex z) { return cargf(z); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_cf(fut, ref_cargf, -120, 120, 1, 20000000);
    bad |= sample_cf(fut, ref_cargf, -8, 8, 1, 10000000);
    bad |= sample_cf_box(fut, ref_cargf, 60, 120, -120, -60, 1, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_cargf, -120, 120, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
