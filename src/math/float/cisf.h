#include "kernel/sincosf.h"
#include <complex.h>

static double _Complex _cisf(float t)
{
    double r;
    unsigned q = __rem_pio2f(t, &r);
    double x = _kernel_cosf(r);
    double y = _kernel_sinf(r);

    switch (q & 3) {
        case 1:
            return CMPLX(-y, x);
        case 2:
            return CMPLX(-x, -y);
        case 3:
            return CMPLX(y, -x);
    }

    return CMPLX(x, y);
}
