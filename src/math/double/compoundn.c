#include "kernel/lncells.h" /* lncells_ln1p_raw_ + the exptab_* exp kernel */
#include "kernel/compoundacc.h"
#include <math.h>
#include <stdint.h>

/* C23 compoundn: (1 + x)^n, correctly rounded.
 *
 * Ported from metallic-rs f64/pow.rs `compound` (~line 557), re-based on this
 * library's pow.c architecture: a lean exp(n·ln(1+x)) double-double leg under
 * a Ziv gate, where ln(1+x) comes from log1p.c's exact-split table leg
 * (lncells_ln1p_raw_, < 2^-66 absolute) instead of metallic-rs's log2 chain.
 * Straddles and the over/underflow / near-boundary edges defer to the
 * CORE-MATH cascade with a log1p front end (kernel/compoundacc.h).
 *
 * The C23 exponent is an integer, which metallic-rs's f64 y is not: (double)n
 * is inexact for |n| > 2^53.  Such n with |x| >= 2^-40 force |n·ln(1+x)| >
 * 2^12, far beyond the finite range, so the sign decides directly; the
 * remaining tiny-x band carries n exactly through the cascade's integer
 * multiplies. */

/* Reduced-argument exp boundaries in natural-log units (= k·ln2), as pow.c. */
#define COMPOUND_OVF 710.4758600739439   /* 1025·ln2: gross overflow → +∞ */
#define COMPOUND_UNF (-745.1332191019412) /* −1075·ln2: gross underflow → +0 */
#define COMPOUND_HI 709.0895657128241    /* 1023·ln2: top of the fast normal band */
#define COMPOUND_LO (-708.3964185322641) /* −1022·ln2: bottom of the fast band */

/* Fast leg: exp(y·ln(1+x)) as a double-double accepted by a Ziv gate.  Writes
 * the result to *out and returns 1 when resolved, else 0 (defer to the
 * accurate path).  x is finite > -1, x != 0; y is finite, not in {0, 1}.
 *
 * The slack bounds the [1,2) mantissa error: ln(1+x)'s < 2^-66 absolute slip
 * scaled by y across the exp gives < |y|·2^-65 (mantissa < 2), and 2^-64
 * covers the y-independent dd exp chain; (1+|y|)·2^-64 keeps a >= 2x margin
 * (the same shape metallic-rs certifies in compound_fast_leg_is_sound). */
static int compound_fast_(double x, double y, double *out)
{
    /* Carry 1 + x exactly as s + c via Fast2Sum; s >= 2^-53 for every finite
     * x > -1 (Sterbenz makes 1 + x exact on [-1, -1/2]), so the table leg's
     * domain is safe.  A |y| >= 2^996 would overflow the Dekker split below;
     * with |ln(1+x)| possibly ~2^-1074 such y can still land in range, so
     * defer them (unreachable through compoundn, whose |y| < 2^63). */
    if (fabs(y) >= 0x1p996)
        return 0;

    double s, c;
    if (fabs(x) <= 1.0) {
        s = 1.0 + x;
        c = x - (s - 1.0);
    } else {
        s = x + 1.0;
        c = 1.0 - (s - x);
    }

    /* The raw pair is unnormalized (the value lives in the hi + lo
     * cancellation); the closing Fast2Sum makes l.hi ≈ ln(1+x) so the
     * threshold products below see the real magnitude.  |error| < 2^-66
     * absolute. */
    exptab_sum_ l = lncells_ln1p_raw_(s, c);
    l = exptab_fast2sum_(l.hi, l.lo);
    double slack = (1.0 + fabs(y)) * 0x1p-64;

    /* de ≈ y·ln(1+x) decides gross over/underflow directly (also catching a
     * ±∞ product). */
    double de = y * l.hi;
    if (de - slack > COMPOUND_OVF) {
        *out = HUGE_VAL;
        return 1;
    }
    if (de + slack < COMPOUND_UNF) {
        *out = 0.0;
        return 1;
    }

    /* Leave the boundary bands and the subnormal range to the accurate path,
     * which rounds them exactly. */
    if (!(de + slack < COMPOUND_HI && de - slack > COMPOUND_LO))
        return 0;

    /* e = y·ln(1+x) as a double-double. */
    exptab_sum_ p = exptab_prod_(l.hi, y);
    exptab_sum_ e = exptab_fast2sum_(p.hi, p.lo + l.lo * y);

    /* Reduce e to r = e − n·ln2/128, n = 128·q + j; |r| ≤ ln2/256. */
    double scaled = rint(e.hi * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = n & (EXPTAB_N - 1);
    int64_t q = n >> 7;

    double a = e.hi - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, (r.lo + e.lo) - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    exptab_sum_ m = exptab_mantissa_(j, &q, r); /* 2^(j/128)·exp(r) ∈ [1,2) */

    double lo = m.hi + (m.lo - slack);
    double hi = m.hi + (m.lo + slack);
    if (lo == hi && q >= -1021 && q <= 1022) {
        *out = shift_(lo, q);
        return 1;
    }
    return 0;
}

double compoundn(double x, long long n)
{
    if (x == 0.0)
        return 1.0; /* (1 ± 0)^n = 1 for every n */

    if (n == 0)
        return x < -1.0 ? (x - x) / (x - x) : 1.0; /* domain error survives n = 0 */

    if (x != x)
        return x + x;

    if (x < -1.0)
        return (x - x) / (x - x); /* 1 + x < 0, including −∞ */

    if (x == INFINITY)
        return n > 0 ? INFINITY : 0.0;

    if (x == -1.0)
        return n > 0 ? 0.0 : INFINITY;

    if (n == 1)
        return 1.0 + x; /* exact single rounding */

    double y = (double)n;
    /* (double)n is exact iff it casts back; y = 2^63 (from n near INT64_MAX)
     * is inexact and would make the back-cast undefined. */
    _Bool exact = y != 0x1p63 && (long long)y == n;

    if (!exact && fabs(x) >= 0x1p-40)
        return (x > 0.0) == (n > 0) ? HUGE_VAL : 0.0;

    double r;
    if (exact && compound_fast_(x, y, &r))
        return r;

    return compound_accurate_(x, y, exact, n);
}
