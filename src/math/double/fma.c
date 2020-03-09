#include <math.h>

double fma(double x, double y, double a)
{
    if (x - x || y - y)
        return x * y + a;

    if (a - a)
        return a;

    if (!x || !y)
        return x * y + a;

    if (!a)
        return x * y;
}
