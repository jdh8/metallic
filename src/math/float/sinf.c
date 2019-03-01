#include "kernel/sincosf.h"

float sinf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? _kernel_cosf(y) : _kernel_sinf(y);

    return q & 2 ? -y : y;
}
