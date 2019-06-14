#include "kernel/sincosf.h"
#include <complex.h>

static double _Complex cisf_(float t)
{
    double r;
    unsigned q = __rem_pio2f(t, &r);
    double x = kernel_cosf_(r);
    double y = kernel_sinf_(r);

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
