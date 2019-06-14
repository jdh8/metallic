#include "kernel/sincosf.h"

float sinf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? kernel_cosf_(y) : kernel_sinf_(y);

    return q & 2 ? -y : y;
}
