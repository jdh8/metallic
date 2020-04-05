#include "../kernel/atanh.h"
#include "../../reinterpret.h"
#include <stdint.h>

static double log2f_(double x)
{
    const double log2e = 1.44269504088896340736;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    x = reinterpret(double, i - (exponent << 52));

    return 2 * log2e * kernel_atanh_((x - 1) / (x + 1)) + exponent;
}
