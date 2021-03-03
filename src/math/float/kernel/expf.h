#ifndef METALLIC_KERNEL_EXPF_H
#define METALLIC_KERNEL_EXPF_H

/* Restriction of `expm1f` to [-0.5 ln 2, 0.5 ln 2] */
static double kernel_expf_(double x)
{
    const double c[] = {
        1.000000010775500705,
        5.000000080819903627e-1,
        1.666650523422326531e-1,
        4.166624066361261157e-2,
        8.369150671031008566e-3,
        1.394858354331218335e-3
    };

    return x * (((((c[5] * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}
#endif
