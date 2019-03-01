#include <math.h>

float hypotf(float x, float y)
{
    if (isinf(x) || isinf(y))
        return INFINITY;

    double a = x;
    double b = y;

    return sqrt(a * a + b * b);
}
