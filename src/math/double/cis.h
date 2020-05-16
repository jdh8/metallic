#include "truncate.h"

int __rem_pio2(double x, double y[static 2]);

static double truncate_sin_(double a, double b, int bits, double r[static 1])
{
    const double c[] = {
       -1.6666666666666664626e-01,
        8.3333333333309503628e-03,
       -1.9841269836761008913e-04,
        2.7557316103657515000e-06,
       -2.5051132049739777141e-08,
        1.5918142570874201633e-10
    };

    double s = a + b;
    double x = s * s;
    double xx = x * x;
    double t = (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx)) * (x * s) + b;
    double y = truncate_(a + t, bits);

    *r = y - a + t;
    return y;
}

static double truncate_cos_(double a, double b, int bits, double r[static 1])
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
    double u = 1 - h;
    double xx = x * x;
    double v = (c[0] + c[1] * x) * xx + (c[2] + c[3] * x + (c[4] + c[5] * x) * xx) * (xx * xx) - a * b + (1 - u - h);
    double y = truncate_(u + v, bits);

    *r = y - u + v;
    return y;
}

static void truncate_cis_(double t, int bits, double x[static 2], double y[static 2])
{
    double r[2];
    unsigned q = __rem_pio2(t, r);
    double x1;
    double x0 = truncate_cos_(r[0], r[1], bits, &x1);
    double y1;
    double y0 = truncate_sin_(r[0], r[1], bits, &y1);

    switch (q & 3) {
        case 0:
            x[0] = x0; x[1] = x1;
            y[0] = y0; y[1] = y1;
            break;
        case 1:
            x[0] = -y0; x[1] = -y1;
            y[0] =  x0; y[1] =  x1;
            break;
        case 2:
            x[0] = -x0; x[1] = -x1;
            y[0] = -y0; y[1] = -y1;
            break;
        case 3:
            x[0] =  y0; x[1] =  y1;
            y[0] = -x0; y[1] = -x1;
    }
}
