#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(rsqrtf(0.0f), INFINITY));
    assert(identical(rsqrtf(-0.0f), -INFINITY));
    assert(identical(rsqrtf(INFINITY), 0.0f));
    assert(isnan(rsqrtf(-1.0f)));
    assert(isnan(rsqrtf(-INFINITY)));
    assert(isnan(rsqrtf(NAN)));

    assert(identical(rsqrtf(1.0f), 1.0f));
    assert(identical(rsqrtf(4.0f), 0.5f));
    assert(identical(rsqrtf(0x1p-100f), 0x1p50f));
    assert(identical(rsqrtf(0x1p-149f), 0x1.6a09e6p+74f));

    /* Correctly rounded samples (MPFR-derived). */
    assert(identical(rsqrtf(2.0f), 0x1.6a09e6p-1f));
    assert(identical(rsqrtf(0x1.921fb6p+1f), 0x1.20dd74p-1f));

    /* The double-rounding victim patched after the promoted expression:
     * the subnormal input 0x002f7e2a. */
    assert(identical(rsqrtf(reinterpret(float, (uint32_t)0x002F7E2A)),
                     reinterpret(float, (uint32_t)0x5F5222E0)));
}
