/* 1/pi at the three working precisions of the C23 half-turn inverse trig
 * functions (atanpi, asinpi, acospi, atan2pi): the lift from this repo's
 * radian engines to half-turn results.
 *
 * Literals ported from metallic-rs src/f64_/atan.rs (INV_PI ~ line 491,
 * INV_PI_DINT ~ line 501, INV_PI_TINT ~ line 2473) and re-verified here
 * against an independent ~500-bit rational 1/pi (Machin series, exact
 * Fraction arithmetic): each is the round-to-nearest at its width. */
#ifndef METALLIC_KERNEL_INVPI_H
#define METALLIC_KERNEL_INVPI_H

#include "atan2tint.h"
#include "dint.h"

/* 1/pi as a double-double (CORE-MATH's ONE_OVER_PIH/PIL). */
static const double invpi_hi_ = 0x1.45f306dc9c883p-2;
static const double invpi_lo_ = -0x1.6b01ec5417056p-56;

/* 1/pi as a 128-bit dint: m = RN(2^129/pi), value = (m/2^127)*2^-2. */
static const dint_t invpi_dint_ = { 0, -2,
    ((unsigned __int128)0xa2f9836e4e441529ULL << 64) | 0xfc2757d1f534ddc1ULL };

/* 1/pi as a 192-bit tint: hi:lo = RN(2^193/pi), value = (sig/2^192)*2^-1. */
static const tint_t invpi_tint_ = { 0, -1,
    ((unsigned __int128)0xa2f9836e4e441529ULL << 64) | 0xfc2757d1f534ddc0ULL,
    0xdb6295993c439042ULL };

#endif
