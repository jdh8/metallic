static double small_(double x)
{
    const double c[] = {
        1.2837916709551257312e-01,
       -3.7612638903183728872e-01,
        1.1283791670953835200e-01,
       -2.6866170644845631032e-02,
        5.2239776221102343580e-03,
       -8.5483267907828700427e-04,
        1.2055322563706511568e-04,
       -1.4925340197381970440e-05,
        1.6455893581368259212e-06,
       -1.6282480334568281746e-07,
        1.4086466515983713037e-08,
       -8.5818517438380188677e-10
    };

    double xx = x * x;

    double y = ((((((((((c[11] * xx + c[10]) * xx + c[9]) * xx + c[8]) * xx + c[7]) * xx + c[6])
        * xx + c[5]) * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0];

    if (x < 0.25)
        return 1 - (x + x * y);

    if (x < 0.5)
        return 0.5 - x - x * y + 0.5;

    return 1 - x - x * y;
}
