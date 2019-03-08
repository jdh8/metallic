int __rem_pio2(double x, double y[static 2]);

static double _ncot(double a, double b)
{
    const double c[] = {
        3.3333333333333492367e-1,
        2.2222222222137698456e-2,
        2.1164021181435519926e-3,
        2.1164019344081091584e-4,
        2.1377908192264018931e-5,
        2.1640078658154096338e-6,
        2.2014631795034396635e-7,
        2.1029663345234246239e-8,
        3.1015070999396236603e-9
    };

    double x = a * a;
    double x2 = x * x;
    double x4 = x2 * x2;
    double r = c[0] + c[1] * x + (c[2] + c[3] * x) * x2 + (c[4] + c[5] * x + (c[6] + c[7] * x) * x2) * x4 + c[8] * (x4 * x4);

    return r * a + b + b / x - 1 / a;
}

static double _tan(double a, double b)
{
    const double c[] = {
        3.0000000000000000533,
       -1.2000000000000110074,
       -5.7142857139124891639e-3,
       -2.5396825878632949098e-4,
       -1.2203638839030932120e-5,
       -5.9887097196464592167e-7,
       -2.9367165242627139229e-8,
       -1.6554430183757908574e-9
    };

    double x = a * a;
    double xx = x * x;
    double r = x / (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x + (c[6] + c[7] * x) * xx) * (xx * xx));

    return r * a + (r + x) * b + b + a;
}

double tan(double x)
{
    double y[2];
    unsigned q = __rem_pio2(x, y);

    return (q & 1 ? _ncot : _tan)(y[0], y[1]);
}
