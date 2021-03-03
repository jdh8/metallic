#include "kernel/sinf.h"
#include <complex.h>

static double cos_(double x)
{
    const double c[] = {
        9.9999999995260043694e-1,
       -4.9999999615433475594e-1,
        4.1666616739207634682e-2,
       -1.3886619210252882086e-3,
        2.4379929375956876394e-5
    };

    x *= x;

    return (((c[4] * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0];
}

static double _Complex cisf_(float t)
{
    double r;
    unsigned q = __rem_pio2f(t, &r);
    double x = cos_(r);
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
