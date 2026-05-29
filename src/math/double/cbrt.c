#include "../reinterpret.h"
#include "kernel/exptab.h" /* exptab_sum_, exptab_prod_, exptab_fast2sum_; pulls in shift_ */
#include "normalize.h"
#include <math.h>
#include <stdint.h>

/* Correctly-rounded double cbrt, ported from metallic-rs (f64 `cbrt`).
 *
 * Newton/Halley iterations are self-correcting, so plain double ops suffice
 * there (in metallic-rs they go through the non-fused `mul_add` fallback on
 * targets without hardware FMA, which is exactly `x*y + a`).  Only the final
 * double-double correction needs error-free arithmetic; WASM has no scalar FMA,
 * so it uses the Dekker-split helpers from kernel/exptab.h. */

/* x/y as a double-double, FMA-free.  hi = x/y; the residual x - hi*y is exact
 * via a Dekker two-product of hi*y, then lo = residual/y. */
static inline exptab_sum_ cbrt_from_quotient_(double x, double y)
{
    double hi = x / y;
    exptab_sum_ p = exptab_prod_(hi, y);
    double lo = ((x - p.hi) - p.lo) / y;
    return (exptab_sum_){ hi, lo };
}

/* Double-double s divided by scalar y, FMA-free, mirroring metallic-rs's
 * `Div<f64> for Sum`: hi = s.hi/y; lo = (residual(s.hi - hi*y) + s.lo)/y. */
static inline exptab_sum_ cbrt_div_(exptab_sum_ s, double y)
{
    double hi = s.hi / y;
    exptab_sum_ p = exptab_prod_(hi, y);
    double lo = (((s.hi - p.hi) - p.lo) + s.lo) / y;
    return (exptab_sum_){ hi, lo };
}

double cbrt(double x)
{
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude == 0 || magnitude >= 0x7FF0000000000000)
        return x; /* +-0, +-inf, nan */

    magnitude = normalize_(magnitude);
    int64_t sign = bits & (int64_t)0x8000000000000000;

    /* Scale extreme |x| into [1e-200, 1e200] so the double-double refinement
     * keeps full precision (tiny x loses it, huge x overflows 2*y^3); undo with
     * `coefficient` afterwards.  The 999 shift mirrors the 2^+-999 scaling of x;
     * 333 = 999/3 rescales y. */
    double coefficient = 1.0;

    if (fabs(x) < 1e-200) {
        x *= 0x1p999; /* true multiply: handles subnormal x (shift_ would not) */
        magnitude += (int64_t)999 << 52;
        coefficient = 0x1p-333;
    } else if (fabs(x) > 1e200) {
        x *= 0x1p-999;
        magnitude -= (int64_t)999 << 52;
        coefficient = 0x1p333;
    }

    int64_t i = 0x2A9F7AF196E8E6E8 + magnitude / 3;
    double y = reinterpret(double, sign | i);

    y += (1.0 / 3.0) * (x / (y * y) - y);
    y += (1.0 / 3.0) * (x / (y * y) - y);
    y *= 0.5 + 1.5 * x / (2.0 * y * (y * y) + x);

    exptab_sum_ quotient = cbrt_div_(cbrt_from_quotient_(x, y), y);
    exptab_sum_ sum = exptab_fast2sum_(2.0 * y, quotient.hi);
    sum = cbrt_div_((exptab_sum_){ sum.hi, quotient.lo + sum.lo }, 3.0);

    return coefficient * (sum.hi + sum.lo);
}
