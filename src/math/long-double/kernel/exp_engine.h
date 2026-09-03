#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_EXP_ENGINE_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_EXP_ENGINE_H

#include "uint.h"
#include <stdbool.h>

#define F128_EXP_ZIV_GATE ((u128)32)
#define F128_EXP_GUARD 15
#define F128_EXP_GUARD_MASK (((u128)1 << F128_EXP_GUARD) - 1)
#define F128_EXP_GUARD_HALF ((u128)1 << (F128_EXP_GUARD - 1))

typedef struct {
    int n;
    u128 r;
} f128_exp_fast_result_t;

typedef struct {
    int n;
    u256_t r;
} f128_exp_wide_result_t;

f128_exp_fast_result_t __metallic_f128_exp_fast(int n, u128 fraction);
f128_exp_wide_result_t __metallic_f128_exp2_frame(u384_t frame);
bool __metallic_f128_exp_undecided(int n, u128 significand, u128 gate);
long double __metallic_f128_exp_round(int n, u128 high, u128 low);

#endif
