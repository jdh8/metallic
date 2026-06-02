#include "src/math/float/cexpf.c"
#include "src/math/double/rem_pio2.c"
#include "cmpfr.h"
#include "../../../complex.h"

static float _Complex fut(float _Complex z) { return cexpf(z); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_cf_box(fut, ref_cexpf, -6, 6, -6, 6, 1, 20000000);
    bad |= sample_cf_box(fut, ref_cexpf, -6, 6, 6, 24, 1, 10000000);
    bad |= sample_cf_box(fut, ref_cexpf, -1, 2, -120, 0, 1, 10000000);
    /* |x| in [64,128]: exp overflows float (cos may rescue) and subnormal/zero
     * results for x < 0 -- exercises the overflow/underflow boundaries. */
    bad |= sample_cf_box(fut, ref_cexpf, 6, 7, -2, 10, 1, 10000000);
    bad |= sample_cf_box(fut, ref_cexpf, -7, -6, -2, 10, 1, 10000000);
    /* large |y|: heavy argument reduction. */
    bad |= sample_cf_box(fut, ref_cexpf, -1, 3, 24, 60, 1, 10000000);
    bad |= sample_cf_nearaxis(fut, ref_cexpf, -4, 4, 10000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}
