#include "kernel/expf.h"
#include "kernel/exptabf.h"
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

    /* Fast leg: exp(x) = 2^(n/64 + h), h in log2 units.  a - n is exact
     * (Sterbenz), so h carries the full rounding detail of a = fl(x log2(e))
     * and matches CORE-MATH's reduction bit for bit. */
    double a = x * 0x1.71547652b82fep+6; /* 64 log2(e) */
    double n = rint(a);
    double h = 0x1p-6 * (a - n);
    double r = kernel_exptabf_(h, n);
    float ub = r, lb = r - r * 1.45e-10;

    if (ub == lb)
        return ub;

    /* Ziv fallback.  Cody-Waite reduction: ln2hi has 14 trailing zero bits,
     * so n * ln2hi is exact and no fused multiply-add is needed (cf. the WASM
     * no-FMA model). */
    n = rint(x * log2e);
    double t = x - n * ln2hi;
    double y = 1 + kernel_expf_(t - n * ln2lo);

    return shift_(y, n);
}
