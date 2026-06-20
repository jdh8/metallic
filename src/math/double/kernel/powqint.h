/* Accurate-path 256-bit fixed-point type for the correctly-rounded power
 * function (Ziv's third iteration).
 *
 * Ported from metallic-rs f64/qint.rs (itself a port of Tom Hubrecht's
 * qint64_t arithmetic in CORE-MATH binary64/pow/qint.h).  A qint_t represents
 *   value = (-1)^sgn * (M / 2^255) * 2^ex,
 * with the 256-bit significand M normalized so bit 255 is set for nonzero
 * values.  CORE-MATH stores M as four uint64 words {hh, hl, lh, ll}; here they
 * are packed into two unsigned __int128 words: hi = (hh << 64) | hl (top half),
 * lo = (lh << 64) | ll (bottom half), i.e. M = (hi << 128) | lo.
 *
 * This is exact integer arithmetic carrying ~255 significant bits: every
 * operation is plain integer add/sub/mul, no FMA.  It is the rare last-resort
 * fallback behind the 128-bit dint phase of the accurate pow path. */
#ifndef METALLIC_KERNEL_POWQINT_H
#define METALLIC_KERNEL_POWQINT_H

#include "../../reinterpret.h"
#include <stdint.h>

typedef struct {
    _Bool sgn;
    int64_t ex;
    unsigned __int128 hi; /* top 128 bits (hh:hl) */
    unsigned __int128 lo; /* bottom 128 bits (lh:ll) */
} qint_t;

#endif
