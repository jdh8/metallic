#include <math.h>

static double rint_(double x)
{
    const double rectifier = 0x1p52;
    double r = fabs(x);

    if (r < rectifier) {
        double y = r + rectifier;
        return copysign(y - rectifier, x);
    }

    return x;
}
