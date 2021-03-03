#ifndef METALLIC_KERNEL_FAST_ATANH_H
#define METALLIC_KERNEL_FAST_ATANH_H

/* Inverse hyperbolic tangent restricted to [-c, c], where
 *
 *     √2 - 1                  1 + c
 * c = ------  the solution to ----- = √2.
 *     √2 + 1,                 1 - c
 */
static double kernel_atanh_(double x)
{
    const double c[] = {
        0.33333332822728226129,
        0.20000167595436263505,
        0.14268654271188685375,
        0.11791075649681414150
    };

    double xx = x * x;

    return x + x * xx * (((c[3] * xx + c[2]) * xx + c[1]) * xx + c[0]);
}
#endif
