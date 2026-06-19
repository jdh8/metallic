#include "kernel/trig.h"
#include "kernel/trigdint.h"

double tan(double x)
{
    if (!isfinite(x))
        return x - x;

    double ax = fabs(x);
    if (ax < TRIG_SMALL)
        return x;

    dd_t r;
    int64_t q = trig_rem_pio2_(ax, &r);

    dd_t t = trig_tan_kernel_fast_(r);

    /* tan has period pi: even quadrants give tan(r), odd give -cot(r) = -1/tan(r).
     * Lean reciprocal: seed y = -1/t.hi, fold t.lo into the residual e = 1 + y*t
     * (computed exactly via Dekker for the cancelling y*t.hi product), take
     * y*(1 + e); the dropped y*e^2 ~ 2^-104 is noise to the 2^-59 gate. */
    dd_t fast;
    if (!(q & 1)) {
        fast = t;
    } else {
        double y = -1.0 / t.hi;
        double p = y * t.hi;
        double ah = split_(y), al = y - ah;
        double bh = split_(t.hi), bl = t.hi - bh;
        double pl = ((ah * bh - p) + ah * bl + al * bh) + al * bl;
        double e = ((1.0 + p) + pl) + y * t.lo;
        fast = (dd_t){ y, y * e };
    }

    double mag;
    if (!trig_ziv_(fast, &mag)) {
        dint_t rr;
        int64_t qq = payne_hanek_dint_(ax, &rr);
        dint_t s, c;
        sin_cos_dint_(&rr, &s, &c);
        dint_t num = select_sin_dint_(qq, s, c);
        dint_t den = select_cos_dint_(qq, s, c);
        dint_t rc = recip_dint_(&den);
        dint_t v = dint_mul_(&num, &rc);
        mag = dint_to_f64_(&v);
    }

    return signbit(x) ? -mag : mag;
}
