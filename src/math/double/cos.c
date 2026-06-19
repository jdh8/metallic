#include "kernel/trig.h"
#include "kernel/trigdint.h"

double cos(double x)
{
    if (!isfinite(x))
        return x - x;

    double ax = fabs(x);
    if (ax < TRIG_SMALL)
        return 1.0;

    dd_t r;
    int64_t q = trig_rem_pio2_(ax, &r);

    /* cos(|x|): even quadrants use cos(r), odd use sin(r); (q+1)&2 sets the sign. */
    dd_t fast = (q & 1) ? trig_sin_kernel_fast_(r) : trig_cos_kernel_fast_(r);
    if ((q + 1) & 2)
        fast = trig_neg_(fast);

    double res;
    if (!trig_ziv_(fast, &res)) {
        dint_t rr;
        int64_t qq = payne_hanek_dint_(ax, &rr);
        dint_t v = (qq & 1) ? sin_dint_(&rr) : cos_dint_(&rr);
        if ((qq + 1) & 2)
            v = neg_dint_(v);
        res = dint_to_f64_(&v);
    }

    return res;
}
