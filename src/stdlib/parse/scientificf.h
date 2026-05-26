#include "decimal.h"
#include <stdint.h>

static float scientific_(uint32_t significand, int exp)
{
    return significand * __builtin_powi(10, exp);
}

static float decimal_to_scalar_(const decimal_t* d)
{
    return scientific_((uint32_t)d->mant, d->dec_exp);
}
