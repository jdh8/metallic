static double kernel_sinf_(double x)
{
    const double c[] = {
       -1.666666666666663e-1,
        8.333333333321917e-3,
       -1.9841269829457193e-4,
        2.755731358196805e-6,
       -2.505074230488205e-8,
        1.5895944524342348e-10
    };

    double xx = x * x;

    return x + x * xx * (((((c[5] * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]);
}

int __rem_pio2f(float x, double y[static 1]);
