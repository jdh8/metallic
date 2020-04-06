#include "kernel/exp.h"
#include "shift.h"
#include "truncate.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

static double kernel_(double x)
{
    const double c[] = {
        3.33333333333333325270e-1,
       -2.22222222222133128770e-2,
        2.11640211480687751936e-3,
       -2.11640107146572980826e-4,
        2.13747028936886696782e-5,
       -2.12203166597434505056e-6
    };

    double xx = x * x;

    return c[0] * x + (c[1] + c[2] * x) * xx + (c[3] + c[4] * x + c[5] * xx) * (xx * xx);
}

static double divs_(double c, double a, double b)
{
    double s = truncate_(a + b, 32);
    double x = truncate_(c / (a + b), 27);

    return x + (c - x * s - x * (a - s + b)) / (a + b);
}

static double half_(double x)
{
    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];
    double y = kernel_expb_(a, b);

    switch (reinterpret(uint64_t, n)) {
        case 0x3FF0000000000000:
            x = 2 * y + 1;
            return divs_(x, 2, x);
        case 0x4000000000000000:
            x = 4 * y + 3;
            return divs_(x, 2, x);
    }
    return 1 - 2 / (shift_(y + 1, n) + 1);
}

static double right_(double x)
{
    if (x < 0.2554128118829953416)
        return divs_(x, 1, kernel_(x * x));

    if (x < 19.061547465398495995)
        return half_(2 * x);

    return 1 + 0 / x;
}

double tanh(double x)
{
    return copysign(right_(fabs(x)), x);
}
