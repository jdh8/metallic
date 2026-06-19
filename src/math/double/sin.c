#include "kernel/trig.h"
#include "kernel/trigdint.h"

double sin(double x)
{
    if (!isfinite(x))
        return x - x;

    double ax = fabs(x);
    if (ax < TRIG_SMALL)
        return x;

    dd_t r;
    int64_t q = trig_rem_pio2_(ax, &r);

    /* sin(|x|): even quadrants use sin(r), odd use cos(r); q&2 sets the sign. */
    dd_t fast = (q & 1) ? trig_cos_kernel_fast_(r) : trig_sin_kernel_fast_(r);
    if (q & 2)
        fast = trig_neg_(fast);

    double mag;
    if (!trig_ziv_(fast, &mag)) {
        dint_t rr;
        int64_t qq = payne_hanek_dint_(ax, &rr);
        dint_t v = (qq & 1) ? cos_dint_(&rr) : sin_dint_(&rr);
        if (qq & 2)
            v = neg_dint_(v);
        mag = dint_to_f64_(&v);
    }

    return signbit(x) ? -mag : mag;
}
