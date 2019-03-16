/* Restriction of (x -> (√x * coth(√x / 2) - 2) / x) to [0, (0.5 ln 2)^2] */
static double _kernel_expa(double x)
{
    const double c[] = {
        1.6666666666666661553e-1,
       -2.7777777777564778614e-3,
        6.6137564717940089175e-5,
       -1.6534060280704224368e-6,
        4.1437773411068822733e-8
    };

    double xx = x * x;

    return c[0] * x + (c[1] + c[2] * x) * xx + (c[3] + c[4] * x) * (xx * xx);
}

/* Restriction of expm1(a + b) to [0, 0.5 ln 2]
 *
 * Preferably, |b| < 0x1p-26 * |a|
 */
static double _kernel_expb(double a, double b)
{
    double x = a + b;
    double y = x - _kernel_expa(x * x);

    return x * y / (2 - y) + b + a;
}
