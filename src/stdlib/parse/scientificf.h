#include <stdint.h>

static float _scientific(uint32_t significand, int exp)
{
    return significand * __builtin_powi(10, exp);
}
