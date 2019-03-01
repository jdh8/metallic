#ifndef METALLIC_KERNEL_ATANHF_H
#define METALLIC_KERNEL_ATANHF_H

/* Inverse hyperbolic tangent restricted to [-c, c], where
 *
 *     √2 - 1                  1 + c
 * c = ------  the solution to ----- = √2.
 *     √2 + 1,                 1 - c
 */
static double _kernel_atanhf(double x)
{
    const double c[] = {
        0.9999999993072205474,
        0.3333340818599626478,
        0.1998737838945025914,
        0.1496325726858180278
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}
#endif
