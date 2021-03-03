#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

/* Restriction of (cosh ∘ √) to [0, (0.5 ln 2)^2] */
static double kernel_(double x)
{
    const double c[] = {
        9.99999999999999996932e-1,
        5.00000000000001838974e-1,
        4.16666666664880674341e-2,
        1.38888889523213161709e-3,
        2.48014854761043149319e-5,
        2.76326417118194562359e-7
    };

    return ((((c[5] * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0];
}

double cosh(double x)
{
    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    x = fabs(x);

    if (reinterpret(uint64_t, x) > 0x408633CE8FB9F87D)
        return x * HUGE_VAL;

    if (x < 0.5 * (ln2[0] + ln2[1]))
        return kernel_(x * x);

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];

    double e;
    double s = kernel_expm1_(a, b, &e);

    int64_t exponent = n;
    int64_t head = 0x3FF - 2 * exponent;
    double z = e + s + 1;
    double c = reinterpret(double, (head * (head > 0)) << 52);

    return shift_(c / z + e + s + 1, exponent - 1);
}
