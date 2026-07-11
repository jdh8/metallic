/* C23 atanpif: atan(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `atanpif` (src/f32_/atan.rs): atanf's rational kernel
 * scaled by 1/pi, with the |x| > 1 reflection folding into an exact +-1/2
 * (atanpi(+-inf) = +-1/2 falls out).  The kernel's double rounding cannot
 * steer a handful of f32 ties; the exhaustive 2^32 sweep pins those inputs
 * (odd symmetry covers the negatives) and certifies everything else. */

#include "../reinterpret.h"
#include "kernel/atanf.h"
#include <math.h>
#include <stdint.h>

float atanpif(float x)
{
    switch (reinterpret(uint32_t, x) & (~0u >> 1)) {
    case 0x3F693531:
        return copysignf(reinterpret(float, (uint32_t)0x3E70D331), x);
    case 0x332332E9:
        return copysignf(reinterpret(float, (uint32_t)0x324FCA8F), x);
    case 0x39050B0D:
        return copysignf(reinterpret(float, (uint32_t)0x38296554), x);
    }

    const double inv_pi = 0x1.45f306dc9c883p-2;
    double xd = x;

    if (fabsf(x) > 1.0f) {
        double recip = 1.0 / xd;
        return -recip * inv_pi * kernel_atanf_ratio_(recip) + copysign(0.5, xd);
    }

    return xd * inv_pi * kernel_atanf_ratio_(xd);
}
