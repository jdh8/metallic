#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_ROOTS_H_
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_ROOTS_H_

#include "uint.h"

static inline u128 rsqrt_step_(u128 w, u128 q)
{
    const u128 unit = (u128)1 << 124;
    u128 residual = mhi_(w, mhi_(q, q));
    return residual >= unit
        ? q - (mhi_(q, residual - unit) << 3)
        : q + (mhi_(q, unit - residual) << 3);
}

u128 sqrt_wide_(u128 z);

#endif
