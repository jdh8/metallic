#include "kernel/sincosf.h"

float cosf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? _kernel_sinf(y) : _kernel_cosf(y);

    return (q + 1) & 2 ? -y : y;
}
