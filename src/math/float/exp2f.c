#include "kernel/exp2f.h"
#include "kernel/exptabf.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float exp2f(float x)
{
    if (x < -150)
        return 0;

    if (x > 128)
        return 128 * FLT_MAX;

    /* Fast leg: 2^x = 2^(n/64 + h).  h is exact: n/64 is a power-of-two
     * multiple of n, and the subtraction meets Sterbenz. */
    double n = rint(64 * (double)x);
    double h = x - 0x1p-6 * n;
    double r = kernel_exptabf_(h, n);
    float ub = r, lb = r - r * 0x1.3d8p-33;

    if (ub == lb)
        return ub;

    /* Ziv fallback.  Hard-to-round cases the polynomial path misses by 1 ulp
     * (oracle sweep). */
    if (x == 0x1.853a6ep-9f)
        return 0x1.00870ap+0f;

    if (x == -0x1.e7526ep-6f)
        return 0x1.f58d62p-1f;

    float m = rintf(x);
    double y = 1 + kernel_exp2f_(x - m);

    return shift_(y, m);
}
