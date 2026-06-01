#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float hypotf(float x, float y)
{
    float ax = fabsf(x), ay = fabsf(y);

    if (!isfinite(ax) || !isfinite(ay)) {
        /* hypot(±inf, y) = +inf for any y, even NaN; otherwise propagate NaN. */
        if (isinf(ax) || isinf(ay))
            return INFINITY;
        return ax + ay;
    }

    float big = fmaxf(ax, ay), small = fminf(ax, ay);

    /* x, y promote exactly, so bb and ss are exact doubles. */
    double bd = big, sd = small;
    double bb = bd * bd, ss = sd * sd;

    /* small <= 2^-13 * big => true hypot is within 1/4 ulp(big) above big, so it
     * rounds to big in round-to-nearest (WASM's only mode); no sqrt needed. */
    if (sd < bd * 0x1.fffffep-13)
        return big;

    double r2 = bb + ss;
    double r = sqrt(r2);
    float rf = r;
    uint64_t rbits = reinterpret(uint64_t, r);

    /* Past FLT_MAX the single rounding (float)r already yields the correct
     * FLT_MAX/inf; the bit tests below would read meaningless low bits. */
    if (rbits > 0x47EFFFFFE0000000)
        return rf;

    /* Common case: r's f64 tail is far from an f32 midpoint, so the single
     * rounding is already correct and the exact residual check is skipped. */
    if (((rbits + 1) & 0xFFFFFFF) > 2)
        return rf;

    /* Tight sliver near an f32 midpoint: decide exactly.  bb + ss = r2 + e via
     * TwoSum recovers the bits the sum drops, so S = bb + ss is the exact sum of
     * squares; the half-ulp midpoint m has 25 bits, so m*m is exact and
     * (r2 - m*m) + e gives the correctly-rounded choice against the exact S. */
    double bp = r2 - bb;
    double e = (bb - (r2 - bp)) + (ss - bp);
    double rd = rf;
    double resid = (r2 - rd * rd) + e;

    if (resid == 0)
        return rf;

    /* FLT_MAX may round up to infinity; its outer boundary is 2^128's neighbour. */
    if (rf == 0x1.fffffep127f && resid > 0) {
        double m = 0x1.ffffffp127;
        return (r2 - m * m) + e >= 0 ? INFINITY : rf;
    }

    float nb = nextafterf(rf, resid > 0 ? INFINITY : -INFINITY);
    double m = 0.5 * (rd + (double)nb);
    double resid_m = (r2 - m * m) + e;

    if (resid_m == 0)
        return reinterpret(uint32_t, rf) & 1 ? nb : rf;

    return (resid_m > 0) == (resid > 0) ? nb : rf;
}
