#include "../reinterpret.h"
#include <stdint.h>

static float truncf_(float x)
{
    const int32_t mask = 0xFF800000;
    int32_t bits = reinterpret(int32_t, x);
    int32_t magnitude = bits & 0x7FFFFFFF;

    if (magnitude < 0x3F800000) /* 1 */
        return reinterpret(float, bits & 0x80000000);

    if (magnitude < 0x4B000000) /* 0x1p23 */
        return reinterpret(float, mask >> ((magnitude >> 23) - 127) & bits);

    return x;
}
