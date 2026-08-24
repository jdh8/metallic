#ifndef METALLIC_ORACLE_QUAD_COREMATH_H
#define METALLIC_ORACLE_QUAD_COREMATH_H

/* Keep CORE-MATH's binary128 accurate paths deterministic across CI Clang
 * versions: use portable carry operations and define clz(0), which a few
 * exact/subnormal paths reach even though the builtin leaves it undefined. */
_Static_assert(sizeof(unsigned long) == 8, "CORE-MATH expects 64-bit unsigned long");

static inline unsigned long coremath_addcl_(unsigned long x, unsigned long y,
    unsigned long carry_in, unsigned long *carry_out)
{
    unsigned long sum = x + y;
    unsigned long first = sum < x;
    unsigned long result = sum + carry_in;
    *carry_out = first | (result < sum);
    return result;
}

static inline unsigned long coremath_subcl_(unsigned long x, unsigned long y,
    unsigned long borrow_in, unsigned long *borrow_out)
{
    unsigned long difference = x - y;
    unsigned long first = x < y;
    unsigned long result = difference - borrow_in;
    *borrow_out = first | (difference < borrow_in);
    return result;
}

static inline int coremath_clzl_(unsigned long x)
{
    return x ? __builtin_clzl(x) : 64;
}

static inline int coremath_clzll_(unsigned long long x)
{
    return x ? __builtin_clzll(x) : 64;
}

#define __builtin_addcl coremath_addcl_
#define __builtin_subcl coremath_subcl_
#define __builtin_clzl coremath_clzl_
#define __builtin_clzll coremath_clzll_

#endif
