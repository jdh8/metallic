#include "kernel/expf.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float expf(float x)
{
    const float minimum = -103.972077083991796;
    const float maximum = 88.7228391116729996;

    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return maximum * FLT_MAX;

    float n = rintf(x * log2e);
    double y = 1 + kernel_expf_(x - n * ln2);

    return shift_(y, n);
}
