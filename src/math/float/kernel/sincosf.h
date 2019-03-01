int __rem_pio2f(float x, double y[static 1]);

static double _kernel_sinf(double x)
{
    const double c[] = {
        9.9999999692634277740e-1,
       -1.6666650699202422675e-1,
        8.3320368751598070196e-3,
       -1.9504022000675618505e-4
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}

static double _kernel_cosf(double x)
{
    const double c[] = {
        9.9999997242332292107e-1,
       -4.9999856695848847717e-1,
        4.1655026884251524438e-2,
       -1.3585908510113298585e-3
    };

    x *= x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * (x * x);
}
