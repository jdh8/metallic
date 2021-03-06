#include "../reinterpret.h"
#include <stdint.h>

static double floor_(double x)
{
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude == 0 || magnitude >= 0x4330000000000000) /* 0x1p52 */
        return x;

    if (magnitude < 0x3FF0000000000000) /* 1 */
        return bits >> 63;

    int64_t mask = 0x000FFFFFFFFFFFFF >> ((magnitude >> 52) - 1023);

    return reinterpret(double, (bits + (bits < 0) * mask) & ~mask);
}
