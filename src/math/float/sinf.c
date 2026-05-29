#include "kernel/sincosf.h"

float sinf(float x)
{
    /* sin(±0) = ±0; the kernel would otherwise flip the sign of zero. */
    if (x == 0)
        return x;

    /* Hard-to-round large arguments (exhaustive oracle sweep). */
    if (x == 0x1.33333p+13f)
        return -0x1.63f4bap-2f;

    if (x == -0x1.33333p+13f)
        return 0x1.63f4bap-2f;

    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? kernel_cosf_(y) : kernel_sinf_(y);

    return q & 2 ? -y : y;
}
