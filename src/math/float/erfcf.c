#include "kernel/erff.h"
#include <math.h>

float erfcf(float x)
{
    float r = fabsf(x);

    if (r < 0.4375f) {
        /* lone hard-to-round case in this regime (found by exhaustive sweep) */
        if (x == -0x1.d93ec4p-17f)
            return 0x1.00010ap+0f;

        return 1 - kernel_erf_(x);
    }

    /* lone hard-to-round case in the complement regime */
    if (x == 0x1.1bea34p+1f)
        return 0x1.bfaddap-10f;

    double t = kernel_erfc_(r);

    return signbit(x) ? 2 - t : t;
}
