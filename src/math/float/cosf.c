#include "kernel/sincosf.h"

float cosf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? kernel_sinf_(y) : kernel_cosf_(y);

    return (q + 1) & 2 ? -y : y;
}
