int __rem_pio2(double x, double y[static 2]);

static double kernel_sin_(double a, double b)
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

static double kernel_cos_(double a, double b)
{
    const double c[] = {
        4.1666666666666599556e-02,
       -1.3888888888874138650e-03,
        2.4801587289491835985e-05, 
       -2.7557314355244334647e-07,
        2.0875723684774487669e-09,
       -1.1359669885573144321e-11
    };

    double x = a * a;
    double h = 0.5 * x;
    double y = 1 - h;
    double xx = x * x;

    return (c[0] + c[1] * x) * xx + (c[2] + c[3] * x + (c[4] + c[5] * x) * xx) * (xx * xx) - a * b + (1 - y - h) + y;
}
