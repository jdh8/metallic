#include "kernel/lanczos.h"
#include "finite/log2f.h"
#include "finite/sinpif.h"
#include "exp2f.h"
#include <math.h>

static double gamma1p_(double z)
{
    const double log2e = 1.442695040888963407;

    double base = lanczos_g_ + 0.5 + z;

    return exp2f_((0.5 + z) * log2f_(base) - log2e * base) * lanczos_series_(z);
}

float tgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0)
        return copysignf(INFINITY, z);

    if (z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return NAN;
        return pi / (sinpif_(z) * gamma1p_(-z));
    }

    return gamma1p_(z - 1.0);
}
