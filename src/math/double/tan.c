int __rem_pio2(double x, double y[static 2]);

static double _kernel(double a, double b)
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
    double t = _kernel(y[0], y[1]);

    return q & 1 ? -1 / t : t;
}
