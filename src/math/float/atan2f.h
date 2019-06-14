#include "kernel/atanf.h"
#include <math.h>

static double atan2f_(double y, double x)
{
    const double pi = 3.14159265358979323846;

    double a = fabs(x);
    double b = fabs(y);

    if (a < b)
        return copysign(pi / 2, y) - kernel_atanf_(x / y);

    double acute = a == b ? b ? copysign(pi / 4, y) : y : kernel_atanf_(y / a);

    return signbit(x) ? copysign(pi, y) - acute : acute;
}
