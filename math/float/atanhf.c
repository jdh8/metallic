#include "atanhf/taylor.h"
#include "quietf.h"
#include <math.h>

static float _positive(float x)
{
    const float ln2_2 = 0.34657359028;
    const int32_t mantissa = 0x007FFFFF;
    const int32_t sqrt2 = 0x3FB504F3;

    float y = (1 + x) / (1 - x);
    int32_t word = *(int32_t*)&y;
    int exponent = (word >> 23) - 127;

    word = (word & mantissa) | 0x3F800000;

    if (word >= sqrt2) {
        word &= 0xFF7FFFFF;
        ++exponent;
    }

    y = *(float*)&word;

    switch (exponent) {
        case 0:
            return atanhf_taylor(x);
        case 1:
            return atanhf_taylor((float)(1 - 3.0 * x) / (x - 3)) + ln2_2;
    }
    
    return atanhf_taylor((y - 1) / (y + 1)) + exponent * ln2_2;
}

float atanhf(float x)
{
    float absx = fabsf(x);

    if (absx == 1)
        return copysignf(HUGE_VALF, x);

    if (absx < 1)
        return copysignf(_positive(absx), x);

    return quietf(x);
}
