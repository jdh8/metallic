#ifndef METALLIC_KERNEL_EXP2F_H
#define METALLIC_KERNEL_EXP2F_H

/* Restriction of (x -> 2^x - 1) to [-0.5, 0.5] */
static double kernel_exp2f_(double x)
{
    const double c[] = {
        6.931471880289532425e-1,
        2.402265108421173406e-1,
        5.550357105498874537e-2,
        9.618030771171497658e-3,
        1.339086685300950937e-3,
        1.546973499989028719e-4
    };

    double xx = x * x;

    return x * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx));
}
#endif
