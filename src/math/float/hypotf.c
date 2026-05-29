#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float hypotf(float x, float y)
{
    if (isinf(x) || isinf(y))
        return INFINITY;

    /* x, y promote exactly, so a*a and b*b are exact doubles, and aa + bb = s + e
     * exactly (TwoSum) -- e recovers the bits the sum drops.  S = aa + bb is the
     * exact sum of squares; the result is the correctly-rounded sqrt(S). */
    double a = x, b = y;
    double aa = a * a, bb = b * b;
    double s = aa + bb;

    if (s == 0)
        return 0;

    double bp = s - aa;
    double e = (aa - (s - bp)) + (bb - bp);

    float rf = sqrt(s);

    if (isinf(rf))
        return rf;

    /* rf is the nearest float to sqrt(s) ~ sqrt(S); the true result is rf or its
     * neighbour `nb` toward sqrt(S).  Decide exactly: the half-ulp midpoint m
     * between them has 25 bits, so m*m is exact, and S - m*m (computed as
     * (s - m*m) + e, both terms exact) gives the correctly-rounded choice with
     * no approximation -- the deciding comparison is against the exact S. */
    double rd = rf;
    double resid = (s - rd * rd) + e;

    if (resid == 0)
        return rf;

    /* FLT_MAX may round up to infinity; its outer boundary is 2^128's neighbour. */
    if (rf == 0x1.fffffep127f && resid > 0) {
        double m = 0x1.ffffffp127;
        return (s - m * m) + e >= 0 ? INFINITY : rf;
    }

    float nb = nextafterf(rf, resid > 0 ? INFINITY : -INFINITY);
    double m = 0.5 * (rd + (double)nb);
    double resid_m = (s - m * m) + e;

    if (resid_m == 0)
        return reinterpret(uint32_t, rf) & 1 ? nb : rf;

    return (resid_m > 0) == (resid > 0) ? nb : rf;
}
