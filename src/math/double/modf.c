#include <math.h>

double modf(double x, double i[static 1])
{
    *i = trunc(x);

    return copysign(isinf(x) ? 0 : x - *i, x);
}
