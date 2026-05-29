#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double finite_(double s)
{
    const double ln2 = 0.6931471805599453094;

    double c = sqrt(s * s + 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    if (exponent)
        return 2 * kernel_atanhf_((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * kernel_atanhf_(s / (c + 1));
}

float asinhf(float x)
{
    if (x - x != 0)
        return x;

    float s = fabsf(x);

    /* Hard-to-round large arguments (exhaustive oracle sweep). */
    if (s == 0x1.bacb4ap+24f)
        return copysignf(0x1.1e0696p+4f, x);

    if (s == 0x1.b121a6p+75f)
        return copysignf(0x1.a9a3f2p+5f, x);

    if (s == 0x1.6351d8p+94f)
        return copysignf(0x1.08b512p+6f, x);

    return copysignf(finite_(s), x);
}
