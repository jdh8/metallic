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
        0.3333333333333101,
        0.2000000000565512,
        0.14285712055055372,
        0.11111432482627695,
        0.09070044755352928,
        0.08311617389198807
    };

    double xx = x * x;

    return x + x * xx * (((((c[5] * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]);
}
#endif
