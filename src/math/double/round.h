#include <math.h>

static double round_(double x)
{
    double r = fabs(x);
    double i = trunc(r);

    return copysign(i + (r - i >= 0.5), x);
}
