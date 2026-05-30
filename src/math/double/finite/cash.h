#include "csqrt.h"
#include <complex.h>
#include <math.h>

static double carg_(double x, double y)
{
    return atan2(y, x);
}

/* Complex inverse hyperbolic functions in the first quadrant, using long double
 * arithmetic internally for accuracy.
 *
 * cash_(x, y, carg) = asinh(x + iy)
 * cash_(x, y, atan2) = acosh(y + ix)
 */
static double _Complex cash_(double x, double y, double arg(double, double))
{
    long double lx = x;
    long double ly = y;
    long double xx = lx * lx;
    long double yy = ly * ly;

    if (x < 0.5) {
        if (y < 1) {
            long double s = xx * (2 + xx + 2 * yy);
            long double t = 1 - yy;
            long double u = sqrtl(s + t * t) + t;
            long double dx = sqrtl(0.5L * (u + xx));
            long double dy = lx * ly / dx;
            double re = (double)(0.5L * log1pl(xx + s / u + 2 * (lx * dx + ly * dy)));
            double im = arg((double)(lx + dx), (double)(ly + dy));

            return CMPLX(re, im);
        }
        if (y == 1) {
            long double r = lx * sqrtl(4 + xx);
            long double dx = sqrtl(0.5L * (r + xx));
            long double dy = sqrtl(0.5L * (r - xx));
            double re = (double)(0.5L * log1pl(xx + r + 2 * (lx * dx + dy)));
            double im = arg((double)(lx + dx), (double)(1 + dy));

            return CMPLX(re, im);
        }
    }

    double _Complex z = CMPLX(x, y) + csqrt_(xx - yy + 1, 2 * x * y);
    long double re = creal(z);
    long double im = cimag(z);
    double re_abs = (double)fabsl(re);
    double im_abs = (double)fabsl(im);

    return CMPLX(0.5 * log1p(re_abs * re_abs + im_abs * im_abs - 1),
                 arg(re_abs, im_abs));
}
