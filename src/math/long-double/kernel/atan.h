#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_ATAN_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_ATAN_H

#include "uint.h"
#include <stdbool.h>

/* Shared fixed-point plumbing for the binary128 inverse-trigonometric family. */
#define ATAN_GUARD_ 15
#define ATAN_GUARD_MASK_ (((u128)1 << ATAN_GUARD_) - 1)
#define ATAN_GUARD_HALF_ ((u128)1 << (ATAN_GUARD_ - 1))
#define ATAN_ZIV_GATE 64

typedef struct {
    u128 numerator;
    u128 denominator;
    int scale;
    bool negative;
    unsigned sector;
    unsigned quadrant;
    bool negate;
} atan_reduction_t;

typedef struct {
    u128 fraction;
    int exponent;
} atan_frac128_t;

/* (2^254/d)(1-delta), with 0 <= delta < 2^-125. */
static inline u128 atan_recip_128_(u128 d)
{
    uint64_t dh = d >> 64;
    uint64_t dl = d;
    uint64_t r = ((u128)1 << 127) / dh - 2;
    u128 e = ((u128)1 << 127)
        - (umulditi3_(dh, r) + (umulditi3_(dl, r) >> 64));
    u128 c = (umulditi3_(r, (uint64_t)e) >> 64)
        + umulditi3_(r, (uint64_t)(e >> 64));
    u128 c2 = umulditi3_((uint64_t)(c >> 32), (uint64_t)(e >> 32)) >> 63;
    return ((u128)r << 63) + c + c2 - 1;
}

u128 __metallic_atan_shr_round(u128 x, unsigned shift);
u256_t __metallic_atan_place(u128 fraction, int exponent);
atan_frac128_t __metallic_atan_combine(u256_t theta, unsigned sector, bool negative,
    unsigned quadrant, bool negate);
atan_frac128_t __metallic_atan_fast(const atan_reduction_t *r);
bool __metallic_atan_round_fast(u128 fraction, int exponent, u128 sign,
    long double *result);
u384_t __metallic_atan_fraction_384(u384_t t, int exponent);
long double __metallic_atan_assemble_384(u384_t theta, bool negative, unsigned sector,
    unsigned quadrant, bool negate, u128 sign);
long double __metallic_atan_round_384(u384_t fraction, int exponent, u128 sign);

#endif
