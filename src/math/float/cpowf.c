#include "clogf.h"
#include "cisf.h"
#include "expf.h"

float _Complex cpowf(float _Complex base, float _Complex exp)
{
    double _Complex z = exp * _clogf(base);
    double x = z;
    double y = cimag(z);

    if (y == 0)
        return CMPLX(_expf(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLX(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return _expf(x) * _cisf(y);
}
