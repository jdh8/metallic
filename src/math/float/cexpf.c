#include "cisf.h"
#include "expf.h"

float _Complex cexpf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (y == 0)
        return CMPLXF(_expf(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLXF(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return _expf(x) * _cisf(y);
}
