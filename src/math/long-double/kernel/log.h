#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_LOG_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_LOG_H

#include "uint.h"
#include <stdbool.h>

/* Base-2 logarithm state shared with powl. */
typedef struct {
    unsigned index;
    u384_t reduction;
    u256_t magnitude;
    bool negative;
} log2_fast_result_;

typedef struct {
    bool negative;
    u384_t magnitude;
    int exponent;
} log2_wide_result_;

log2_fast_result_ __metallic_log2_fast(u128 mantissa, int exponent);
log2_wide_result_ __metallic_log2_wide(int exponent, unsigned index,
    u384_t reduction);

#endif
