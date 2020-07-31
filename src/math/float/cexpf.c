#include "cisf.h"
#include "expf.h"

float _Complex cexpf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    double r = expf_(x);

    if (y == 0)
        return CMPLXF(r, y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLXF(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return r * cisf_(y);
}
