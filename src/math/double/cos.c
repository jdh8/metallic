#include "kernel/sincos.h"

double cos(double x)
{
    double y[2];
    unsigned q = __rem_pio2(x, y);

    x = q & 1 ? _kernel_sin(y[0], y[1]) : _kernel_cos(y[0], y[1]);

    return (q + 1) & 2 ? -x : x;
}
