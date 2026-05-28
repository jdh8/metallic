#ifndef METALLIC_KERNEL_EXPF_H
#define METALLIC_KERNEL_EXPF_H

/* Restriction of `expm1f` to [-0.5 ln 2, 0.5 ln 2]
 *
 * The coefficients approximate (exp(x) - 1) / x; the caller multiplies by x
 * (implicit below) and adds 1.  Degree 10 makes the binary32 result correctly
 * rounded when computed through this f64 intermediate. */
static double kernel_expf_(double x)
{
    const double c[] = {
        1.0,
        5.000000000000006e-1,
        1.6666666666666607e-1,
        4.1666666666573884e-2,
        8.333333333377178e-3,
        1.388888893226473e-3,
        1.9841269746922376e-4,
        2.4801504596496194e-5,
        2.755738188469386e-6,
        2.762626468193145e-7,
        2.506206487727576e-8
    };

    return x * ((((((((((c[10] * x + c[9]) * x + c[8]) * x + c[7]) * x + c[6]) * x + c[5]) * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}
#endif
