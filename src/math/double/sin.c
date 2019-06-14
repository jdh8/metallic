#include "kernel/sincos.h"

double sin(double x)
{
    double y[2];
    unsigned q = __rem_pio2(x, y);

    x = q & 1 ? kernel_cos_(y[0], y[1]) : kernel_sin_(y[0], y[1]);

    return q & 2 ? -x : x;
}
