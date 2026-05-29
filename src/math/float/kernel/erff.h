#ifndef METALLIC_KERNEL_ERFF_H
#define METALLIC_KERNEL_ERFF_H

/* Shared building blocks for erff and erfcf.  erfc IS the complement that erf
 * folds in via 1 - erfc, so both functions are built from the same two pieces:
 * a small-argument erf polynomial and a tail erfc evaluated through exp. */

#include <math.h>

/* erf(x) on |x| < 0.4375 as x*P(x^2), P a degree-7 polynomial in x^2 (CORE-MATH
 * coefficients).  c[0] is the correctly-rounded 2/sqrt(pi): the leading term
 * must be exact so the tiny-argument regime erf(x) ~ (2/sqrt(pi))*x rounds
 * correctly. */
static double kernel_erf_(double x)
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
 * polynomial.  Q(t) = x^2 + log(erfc(x)*(2+x)/2) is smooth and O(1).  The
 * double exp() (not the float kernel) is needed so the tail keeps erfc's
 * relative accuracy, which erfcf returns directly.  Coefficients from:
 *   ratapprox --function="(2/x-2)^2 + log(erfc(2/x-2)) - log(x)"
 *     --dom="[0.16,0.8205]" --num="[1,x,...,x^17]" --den="[1]"
 *     --weight=1 --dispCoeff=hex      (absolute error ~2^-47)
 * The domain reaches down to t = 0.16 (x ~ 10.5), past which erfc(x) is below
 * the binary32 underflow threshold and the exp argument drives the result to 0. */
static double kernel_erfc_(double x)
{
    const double c[] = {
       -0x1.43f898904fd44p+0,
        0xf.fffac8127f2ep-4,
        0x6.00778113cff2p-4,
        0x1.4ec86e9731634p-4,
       -0x1.20f1aefa07d2ap-4,
       -0x4.066c9a3c094b8p-4,
        0x7.9fcc5b1a3b44p-4,
       -0x2.44e93d3a9e3b4p+0,
        0x7.55fbdb9acc25cp+0,
       -0x1.160e29d1d8428p+4,
        0x2.0aeab424dd8ap+4,
       -0x2.e5e9a0ddde61p+4,
        0x2.f5e99b2e46fap+4,
       -0x2.1db82ac6bb75cp+4,
        0x1.058da27a33d88p+4,
       -0x4.fc93795f8e438p+0,
        0xd.50cd9f6f2deep-4,
       -0xd.2db5ecd717e6p-8
    };

    double t = 2 / (2 + x);
    double q = c[17];

    for (int i = 16; i >= 0; --i)
        q = q * t + c[i];

    return t * exp(q - x * x);
}

#endif
