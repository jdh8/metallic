/* Restriction of expm1(a + b) to [0, 0.5 ln 2]
 *
 * Preferably, |b| < 0x1p-26 * |a|
 */
static double kernel_expb_(double a, double b)
{
    const double c[] = {
        1.6666666666666661553e-1,
       -2.7777777777564778614e-3,
        6.6137564717940089175e-5,
       -1.6534060280704224368e-6,
        4.1437773411068822733e-8
    };

    double x = a + b;
    double xx = x * x;
    double y = x - xx * ((((c[4] * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]);

    return x * y / (2 - y) + b + a;
}
