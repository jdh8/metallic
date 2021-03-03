#include "kernel/exp.h"
#include "divs.h"
#include "shift.h"
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

    return x * (((((c[5] * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
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
