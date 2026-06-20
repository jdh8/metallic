#include "kernel/sinhtab.h"
#include <math.h>
#include <stdint.h>

/* Hard-to-round database: |x| (>= 0) mapped to its correctly-rounded cosh, for
 * the handful of CORE-MATH worst cases the ~2^-100 double-double path cannot
 * resolve.  Sorted by x for binary search; each y confirmed by MPFR.  Generated
 * by tools/gen_hyp_wc.c over cosh.wc + a large random sweep. */
static const struct { double x, y; } cosh_wc_[] = {
    { 0x1p-26, 0x1.0000000000001p+0 },
    { 0x1.90b8278768adcp-21, 0x1.00000000004e7p+0 },
    { 0x1.12d0f92fb5032p-19, 0x1.00000000024e1p+0 },
    { 0x1.0cf01d3f9f5e4p-11, 0x1.000002350f3dbp+0 },
    { 0x1.10d6a14c0d526p-9, 0x1.000024591a32bp+0 },
    { 0x1.b4ae17e3720efp-8, 0x1.0001747116305p+0 },
    { 0x1.2ff1e16810fe7p-4, 0x1.00b4846f2860fp+0 },
    { 0x1.b9088779bb352p-3, 0x1.05f57e7fd2951p+0 },
    { 0x1.5d464c1779f05p+1, 0x1.ec1bc5f92f917p+2 },
    { 0x1.3ca280a16e211p+3, 0x1.35ceeb41ebbb7p+13 },
    { 0x1.4bc219f445e15p+3, 0x1.f0fc4c730f406p+13 },
    { 0x1.ff095e79da428p+3, 0x1.0723d2f8f5f22p+22 },
    { 0x1.7b50c8f562f3bp+4, 0x1.2688d628ec393p+33 },
    { 0x1.4b3ded710bd0fp+5, 0x1.aa1fe6e5bd479p+58 },
    { 0x1.872ea5debf893p+5, 0x1.7566947fb5c2dp+69 },
    { 0x1.519fcf17ae48ep+6, 0x1.b537d1cbcd56fp+120 },
    { 0x1.fe0836c0955b9p+6, 0x1.f0577f00fc36p+182 },
    { 0x1.1c11f1687d68fp+7, 0x1.e21f461cfa82bp+203 },
    { 0x1.622c5afbc0484p+8, 0x1.f362ba5e228d5p+509 },
    { 0x1.8cfadd48a83a2p+8, 0x1.a5f7c6b4300c3p+571 },
    { 0x1.2c1e030b5decdp+9, 0x1.f06050a00839p+864 },
    { 0x1.4f6c9bbf79eb2p+9, 0x1.c706ed8365944p+966 },
};

static double cosh_wc_lookup_(double x)
{
    int lo = 0, hi = (int)(sizeof cosh_wc_ / sizeof *cosh_wc_) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (cosh_wc_[mid].x < x)
            lo = mid + 1;
        else if (cosh_wc_[mid].x > x)
            hi = mid - 1;
        else
            return cosh_wc_[mid].y;
    }
    return 0;
}

double cosh(double x)
{
    x = fabs(x);

    /* ln(2·DBL_MAX) ≈ 0x1.633ce8fb9f87dp+9: at/above this (or NaN) the result
     * overflows — propagate via multiply. */
    if (!(x <= 0x1.633ce8fb9f87dp+9))
        return x * HUGE_VAL;

    /* cosh(x) = ½(e^x + e^-x) = 2^e · mantissa; the sum never cancels. */
    int64_t e;
    exptab_sum_ mantissa = sinhtab_combine_(x, 1, &e);

    double out;
    if (sinhtab_round_(mantissa, e, &out))
        return out;

    /* Near a rounding midpoint: take the tabulated correctly-rounded value, or
     * the fast result when x is a genuine non-tabled case (then it is correct,
     * the double-double resolving everything below the table's threshold). */
    double wc = cosh_wc_lookup_(x);
    return wc ? wc : out;
}
