#include "clogf.h"
#include "cisf.h"
#include "expf.h"

float _Complex cpowf(float _Complex base, float _Complex exp)
{
    double _Complex z = exp * clogf_(base);
    double x = z;
    double y = cimag(z);

    if (y == 0)
        return CMPLX(expf_(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLX(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return expf_(x) * cisf_(y);
}
