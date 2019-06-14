#include <complex.h>

static Complex complex_(Real x, Real y)
{
    return _Generic(x,
        float: CMPLXF(x, y),
        double: CMPLX(x, y),
        long double: CMPLXL(x, y)
    );
}

