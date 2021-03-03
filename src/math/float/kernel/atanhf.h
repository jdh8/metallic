#ifndef METALLIC_KERNEL_ATANHF_H
#define METALLIC_KERNEL_ATANHF_H

/* Inverse hyperbolic tangent restricted to [-c, c], where
 *
 *     √2 - 1                  1 + c
 * c = ------  the solution to ----- = √2.
 *     √2 + 1,                 1 - c
 */
static double kernel_atanhf_(double x)
{
    const double c[] = {
        0.33333388190810157475,
        0.19988770787788511120,
        0.14935863879449999474
    };

    double xx = x * x;

    return x + x * xx * ((c[2] * xx + c[1]) * xx + c[0]);
}
#endif
