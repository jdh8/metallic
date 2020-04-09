static double kernel_sinf_(double x)
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

int __rem_pio2f(float x, double y[static 1]);
