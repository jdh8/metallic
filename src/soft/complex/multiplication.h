#include "complex.h"
#include <tgmath.h>

Complex FUNCTION(Real a, Real b, Real c, Real d)
{
    Real ac = a * c;
    Real ad = a * d;
    Real bc = b * c;
    Real bd = b * d;

    Real x = ac - bd;
    Real y = ad + bc;

    if (x != x && y != y) {
        _Bool recalc = 0;

        if (isinf(a) || isinf(b)) {
            a = copysign(!!isinf(a), a);
            b = copysign(!!isinf(b), b);
            if (c != c) c = copysign(0, c);
            if (d != d) d = copysign(0, d);
            recalc = 1;
        }

        if (isinf(c) || isinf(d)) {
            c = copysign(!!isinf(c), c);
            d = copysign(!!isinf(d), d);
            if (a != a) a = copysign(0, a);
            if (b != b) b = copysign(0, b);
            recalc = 1;
        }

        if (!recalc && (isinf(ac) || isinf(bd) || isinf(ad) || isinf(bc))) {
            if (a != a) a = copysign(0, a);
            if (b != b) b = copysign(0, b);
            if (c != c) c = copysign(0, c);
            if (d != d) d = copysign(0, d);
            recalc = 1;
        }

        if (recalc)
            return INFINITY * complex_(a * c - b * d, a * d + b * c);
    }

    return complex_(x, y);
}
