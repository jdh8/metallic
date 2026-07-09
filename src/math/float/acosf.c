#include "kernel/asin.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float acosf(float x)
{
    const double pi = 3.14159265358979323846;
    const double pi_2 = 1.57079632679489661923;
    double a = fabs((double)x);

    /* Fast leg ported from CORE-MATH's cr_acosf (src/binary32/acos/acosf.c,
     * MIT, (c) 2023 Alexei Sibidanov): for |x| < 0x1.c2a1dcp-1 a degree-31
     * odd polynomial x·P(x²) approximates asin(x) in plain doubles — no FMA,
     * matching the wasm target, so CORE-MATH's proven error bound carries
     * over verbatim.  The two constants below are π/2 offset by ± that
     * bound: if both float roundings of π/2 − r agree, that float is the
     * correctly-rounded acos(x); otherwise (and for |x| ≥ 0x1.c2a1dcp-1,
     * including ±∞ and NaN) fall through to the exact-reduction paths. */
    if ((reinterpret(uint32_t, x) << 1) < 0x7EC2A1DC) {
        const double b[] = {
            0x1.fffffffd9ccb8p-1,  0x1.5555c94838007p-3,
            0x1.32ded4b7c20fap-4,  0x1.8566df703309ep-5,
           -0x1.980c959bec9a3p-6,  0x1.56fbb04998344p-1,
           -0x1.403d8e4c49f52p+2,  0x1.b06c3e9f311eap+4,
           -0x1.9ea97c4e2c21fp+6,  0x1.200b8261cc61bp+8,
           -0x1.2274c2799a5c7p+9,  0x1.a558a59cc19d3p+9,
           -0x1.aca4b6a529ffp+9,   0x1.228744703f813p+9,
           -0x1.d7dbb0b322228p+7,  0x1.5c2018c0c0105p+5
        };

        double z = x, z2 = z * z, z4 = z2 * z2, z8 = z4 * z4, z16 = z8 * z8;
        double r = z * ((((b[0] + z2 * b[1]) + z4 * (b[2] + z2 * b[3]))
                       + z8 * ((b[4] + z2 * b[5]) + z4 * (b[6] + z2 * b[7])))
                + z16 * (((b[8] + z2 * b[9]) + z4 * (b[10] + z2 * b[11]))
                       + z8 * ((b[12] + z2 * b[13]) + z4 * (b[14] + z2 * b[15]))));
        float ub = 0x1.921fb54574191p+0 - r;
        float lb = 0x1.921fb543118ap+0 - r;

        if (ub == lb)
            return ub;
    }

    /* |x| < ½: acos(x) = π/2 − asin(x). */
    if (a < 0.5) {
        /* Hard-to-round cases near π/2 the polynomial paths miss by 1 ulp. */
        if (x == 0x1.110b46p-26f)
            return 0x1.921fb6p+0f;

        if (x == 0x1.04c444p-12f)
            return 0x1.920f6ap+0f;

        return pi_2 - kernel_asin_(x, (double)x * x);
    }

    /* |x| ≥ ½: reflect through s = √((1−|x|)/2) ∈ [0, ½], giving
     *   acos(x) = 2·asin(s)        for x ≥ 0
     *   acos(x) = π − 2·asin(s)    for x < 0.
     * For |x| > 1 the radicand is negative, so sqrt yields NaN as required. */
    double u = 0.5 * (1.0 - a);
    double two = 2.0 * kernel_asin_(sqrt(u), u);

    return x > 0 ? two : pi - two;
}
