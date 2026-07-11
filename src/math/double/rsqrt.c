/* C23 rsqrt: 1/sqrt(x), correctly rounded.
 *
 * Port of metallic-rs `rsqrt` (src/f64_/misc.rs), itself CORE-MATH's cr_rsqrt
 * reshaped: seed (1/x)*sqrt(x), one Newton step to a double-double at
 * ~2^-104 relative, a bit-pattern Ziv window, and an exact 128-bit integer
 * refinement for the straddles.  The result is never subnormal (outputs span
 * ~[2^-512, 2^537]), so plain rounding suffices.
 *
 * WASM adaptation: the reference takes the Newton residuals from FMA, which
 * is a software routine here, and a naive Dekker substitute would overflow
 * splitting x > ~2^996.  Instead the input is frame-reduced exactly to
 * x = m * 4^k with m in [1, 4): every product then stays near 1, the Dekker
 * residuals are bit-identical to the FMA ones, and the final 2^-k rescale is
 * exact, so the certified error budget and Ziv window carry over unchanged. */

#include "../reinterpret.h"
#include "shift.h"
#include "split.h"
#include <math.h>
#include <stdint.h>

/* Exact integer refinement of the inputs whose fast-leg residual sits too
 * close to a rounding boundary (port of CORE-MATH's as_rsqrt_refine,
 * round-to-nearest arm).  With the exponent parity folded into a shift,
 * rf rounds correctly iff (rm -+ n)^2 * am brackets 2^127; the walk is
 * carried in wrapping 128-bit fixed point, and a final midpoint test
 * (rm + 1/2)^2 * am < 2^127 settles the nearest neighbour.  `m` is the
 * frame-reduced input in [1, 4), whose mantissa and exponent parity match
 * the original x by construction. */
static double rsqrt_refine_(double rf, double m)
{
    uint64_t ia = reinterpret(uint64_t, m);

    /* m = 1, i.e. x = 4^k: mantissa 0 with odd biased exponent -- exact. */
    if (ia << 11 == 1ULL << 63)
        return rf;

    uint64_t ir = reinterpret(uint64_t, rf);
    uint64_t e = (ia >> 52) & 1;
    uint64_t rm = (ir << 11 | 1ULL << 63) >> 11;
    uint64_t am = ((ia & (~0ULL >> 12)) | 1ULL << 52) << (5 - e);

    /* rrt = rm^2 * am - 2^127 in wrapping fixed point; its sign bit tells
     * whether the candidate rf sits below or above 1/sqrt(x). */
    unsigned __int128 rt = (unsigned __int128)rm * am;
    unsigned __int128 low = (unsigned __int128)(uint64_t)rt * rm;
    uint64_t t1 = (uint64_t)(low >> 64) + (uint64_t)(rt >> 64) * rm;
    unsigned __int128 rrt = (unsigned __int128)t1 << 64 | (uint64_t)low;

    uint64_t s = (uint64_t)(rrt >> 127);
    uint64_t step = 1 - 2 * s; /* +-1-ulp step toward the root */
    unsigned __int128 rts = ((rt << 1) ^ -(unsigned __int128)s) + s;
    uint64_t am2 = am << 1;
    uint64_t am20 = -am;
    unsigned __int128 prrt;

    /* Walk ir one ulp at a time, updating rm^2 * am incrementally by the
     * exact difference +-2*rm*am - (2n - 1)*am, until the residual flips. */
    do {
        ir -= step;
        prrt = rrt;
        am20 += am2;
        rrt -= rts - (unsigned __int128)am20;
    } while (!((prrt ^ rrt) >> 127));

    /* Keep the bracketing value below the root and its candidate ir. */
    if (!(rrt >> 127)) {
        ir += step;
        rrt = prrt;
    }

    /* Round to nearest: bump ir iff the midpoint (rm + 1/2)^2 * am is still
     * below 2^127, i.e. rrt + rm*am + am/4 keeps the sign bit set. */
    rm = (ir << 11 | 1ULL << 63) >> 11;
    rt = (unsigned __int128)rm * am;
    rrt += (unsigned __int128)(am >> 2) + rt;
    return reinterpret(double, ir + (uint64_t)(rrt >> 127));
}

double rsqrt(double x)
{
    uint64_t ix = reinterpret(uint64_t, x);

    if (ix << 1 == 0)
        return 1.0 / x; /* pole at +-0, sign preserved like 1/x */

    if (ix >= 0x7FFULL << 52) { /* +NaN, +inf, or sign bit set */
        if (x != x)
            return x + x; /* quiet NaN, payload preserved */

        if (ix == 0x7FFULL << 52)
            return 0.0; /* +inf */

        return (x - x) / (x - x); /* x < 0, -inf included: NaN */
    }

    /* Frame reduction: x = m * 4^k exactly, m in [1, 4). */
    int64_t k = 0;

    if (ix < 1ULL << 52) { /* subnormal: 2^108 is even and exact */
        x *= 0x1p108;
        ix = reinterpret(uint64_t, x);
        k = -54;
    }

    int64_t e = (int64_t)(ix >> 52) - 1023;
    int64_t p = e & 1;
    k += (e - p) >> 1;

    double m = reinterpret(double, (ix & (~0ULL >> 12)) | (uint64_t)(1023 + p) << 52);

    /* Seed: division and square root issue together, ~1.5 ulp. */
    double r = (1.0 / m) * sqrt(m);

    /* One Newton step with the products kept exact: h ~ r^2*m - 1 from the
     * Dekker residuals, rf + dr is the refined double-double result, and
     * after the exact r - rf swap, dr is the residual beyond rf's rounding. */
    double rh = split_(r), rl = r - rh;
    double mh = split_(m), ml = m - mh;
    double rm = r * m;
    double drm = (rh * mh - rm + rh * ml + rl * mh) + rl * ml;
    double sh = split_(rm), sl = rm - sh;
    double th = r * rm;
    /* th - 1 is exact by Sterbenz (th in (1/2, 2)), so (th - 1) + dth rounds
     * once, bit-identical to the reference fma(r, rm, -1). */
    double dth = (rh * sh - th + rh * sl + rl * sh) + rl * sl;
    double h = (th - 1.0) + dth + r * drm;
    double dr = (r * 0.5) * h;
    double rf = r - dr;
    dr -= r - rf;

    /* Ziv gate on bit patterns (CORE-MATH's shape, window re-derived in
     * metallic-rs): aidr is |dr| rebased to the top of rf's binade, so
     * 0x3c9... corresponds to a residual of exactly half an ulp -- the
     * round-to-nearest tie.  A residual within +-64 bit-units of that
     * midpoint, an implausibly large one, or a vanishing one (an exact
     * result rebased out of range) cannot be certified and takes the exact
     * refinement (rate ~2^-45). */
    uint64_t aidr = (reinterpret(uint64_t, dr) & ~0ULL >> 1)
                  - (reinterpret(uint64_t, rf) & 0x7FFULL << 52)
                  + (0x3FEULL << 52);
    uint64_t mid = (aidr - (0x3C90000000000000 - 64)) >> 7;

    if (mid == 0 || aidr < 0x39BULL << 52 || aidr > (0x3CAULL << 52) - 0x80)
        rf = rsqrt_refine_(rf, m);

    return shift_(rf, -k); /* exact: the result is never subnormal */
}
