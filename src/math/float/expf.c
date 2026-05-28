#include "kernel/expf.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float expf(float x)
{
    const double minimum = -103.97207708399179;
    const double maximum = 88.7228391116729996;

    const double log2e = 1.4426950408889634;
    const double ln2hi = 0.6931471805601177;
    const double ln2lo = -1.7239444525614835e-13;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return maximum * FLT_MAX;

    /* Cody-Waite reduction: ln2hi has 14 trailing zero bits, so n * ln2hi is
     * exact and no fused multiply-add is needed (cf. the WASM no-FMA model). */
    double n = rint(x * log2e);
    double r = x - n * ln2hi;
    double y = 1 + kernel_expf_(r - n * ln2lo);

    return shift_(y, n);
}
