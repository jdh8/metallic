#include "expf.h"
#include <math.h>

/* erf(x) on |x| < 0.4375 as x*P(x^2), P a degree-7 polynomial in x^2 (CORE-MATH
 * coefficients).  c[0] is the correctly-rounded 2/sqrt(pi): the leading term
 * must be exact so the tiny-argument regime erf(x) ~ (2/sqrt(pi))*x rounds
 * correctly. */
static double kernel_(double x)
{
    const double c[] = {
        0x1.20dd750429b6dp+0,
       -0x1.812746b0375fbp-2,
        0x1.ce2f219fd6f45p-4,
       -0x1.b82ce2cbf0838p-6,
        0x1.565bb655adb85p-8,
       -0x1.c025bfc879c94p-11,
        0x1.f81718f61309cp-14,
       -0x1.cc67bd88f5867p-17
    };

    double xx = x * x, x4 = xx * xx, x8 = x4 * x4;
    double c0 = c[0] + xx * c[1];
    double c2 = c[2] + xx * c[3];
    double c4 = c[4] + xx * c[5];
    double c6 = c[6] + xx * c[7];

    c0 += x4 * c2;
    c4 += x4 * c6;

    return x * (c0 + x8 * c4);
}

/* erfc(x) on x >= 0.4375 as t*exp(Q(t) - x^2), t = 2/(2+x), Q a degree-17
 * polynomial.  Q(t) = x^2 + log(erfc(x)*(2+x)/2) is smooth and O(1); the
 * complement is then folded into erf by 1 - erfc.  Coefficients from:
 *   ratapprox --function="(2/x-2)^2 + log(erfc(2/x-2)) - log(x)"
 *     --dom="[0.28,0.8205]" --num="[1,x,...,x^17]" --den="[1]"
 *     --weight=1 --dispCoeff=hex      (absolute error ~2^-55) */
static double complement_(double x)
{
    const double c[] = {
       -0x1.43f888ee429fep+0,
        0xf.ffd4c3f2f6d2p-4,
        0x6.0328c5cf13acp-4,
        0x1.3094d5392783bp-4,
       -0x3.647184ac4a878p-8,
       -0x9.426ba3017e4b8p-4,
        0x1.e5d726612d67bp+0,
       -0x7.11319372220bp+0,
        0x1.4136b53c7b73p+4,
       -0x2.c234346d37498p+4,
        0x4.d24bcb9472e7p+4,
       -0x6.87a6ebaa05b84p+4,
        0x6.a6201f4b1266p+4,
       -0x4.f75075b050fp+4,
        0x2.a3dc1a7903808p+4,
       -0xf.2bdf678f9c498p+0,
        0x3.4f65a5ea215p+0,
       -0x5.5138f95af69p-4
    };

    double t = 2 / (2 + x);
    double q = c[17];

    for (int i = 16; i >= 0; --i)
        q = q * t + c[i];

    return t * expf_(q - x * x);
}

float erff(float x)
{
    float r = fabsf(x);

    return r < 0.4375f ? kernel_(x) : copysignf(1 - complement_(r), x);
}
