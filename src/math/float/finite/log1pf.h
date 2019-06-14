#include "../kernel/atanhf.h"
#include "../../reinterpret.h"
#include <stdint.h>

static double log1pf_(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, 1 + x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    if (exponent)
        return 2 * kernel_atanhf_((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * kernel_atanhf_(x / (2 + x));
}
