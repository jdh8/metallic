#include "kernel/sincosf.h"

float cosf(float x)
{
    /* Hard-to-round large arguments (exhaustive oracle sweep); cos is even. */
    if (x == 0x1.119ae6p+115f || x == -0x1.119ae6p+115f)
        return 0x1.f3176ap-1f;

    if (x == 0x1.2b9622p+67f || x == -0x1.2b9622p+67f)
        return 0x1.f0285ep-1f;

    if (x == 0x1.3170fp+63f || x == -0x1.3170fp+63f)
        return 0x1.fe2976p-1f;

    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? kernel_sinf_(y) : kernel_cosf_(y);

    return (q + 1) & 2 ? -y : y;
}
