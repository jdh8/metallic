#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

float remainderf(float numerator, float denominator)
{
    uint32_t bits = reinterpret(uint32_t, numerator) & 0x80000000;

    numerator = fmodf(fabsf(numerator), 2 * denominator);
    denominator = fabsf(denominator);

    if (denominator < 2 * FLT_MIN) {
        if (2 * numerator > denominator) {
            numerator -= denominator;
            if (2 * numerator >= denominator)
                numerator -= denominator;
        }
    }
    else if (numerator > 0.5f * denominator) {
        numerator -= denominator;
        if (numerator >= 0.5f * denominator)
            numerator -= denominator;
    }

    bits ^= reinterpret(uint32_t, numerator);

    return reinterpret(float, bits);
}
