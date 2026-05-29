#include "kernel/exp2f.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float exp2f(float x)
{
    if (x < -150)
        return 0;

    if (x > 128)
        return 128 * FLT_MAX;

    /* Hard-to-round cases the polynomial path misses by 1 ulp (oracle sweep). */
    if (x == 0x1.853a6ep-9f)
        return 0x1.00870ap+0f;

    if (x == -0x1.e7526ep-6f)
        return 0x1.f58d62p-1f;

    float n = rintf(x);
    double y = 1 + kernel_exp2f_(x - n);

    return shift_(y, n);
}
