int __rem_pio2(double x, double y[static 2]);

static double _kernel_sin(double a, double b)
{
    const double c[] = {
       -1.6666666666666664626e-01,
        8.3333333333309503628e-03,
       -1.9841269836761008913e-04,
        2.7557316103657515000e-06,
       -2.5051132049739777141e-08,
        1.5918142570874201633e-10
    };

    double y = a + b;
    double x = y * y;
    double xx = x * x;

    return (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx)) * (x * y) + b + a;
}

static double _kernel_cos(double a, double b)
{
    const double c[] = {
        4.1666666666666599556e-02,
       -1.3888888888874138650e-03,
        2.4801587289491835985e-05, 
       -2.7557314355244334647e-07,
        2.0875723684774487669e-09,
       -1.1359669885573144321e-11
    };

    double y = a + b;
    double x = y * y;
    double xx = x * x;

    return (c[0] + c[1] * x) * xx + (c[2] + c[3] * x + (c[4] + c[5] * x) * xx) * (xx * xx) - 0.5 * x + 1;
}
