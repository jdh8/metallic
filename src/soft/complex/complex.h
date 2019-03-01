#include <complex.h>

static Complex _complex(Real x, Real y)
{
    return _Generic(x,
        float: CMPLXF(x, y),
        double: CMPLX(x, y),
        long double: CMPLXL(x, y)
    );
}

