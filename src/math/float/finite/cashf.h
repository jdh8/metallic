#include "csqrt.h"
#include "log1pf.h"
#include "../kernel/atanf.h"
#include "../kernel/atanhf.h"
#include "../../reinterpret.h"
#include <stdint.h>

static double real_clogf_(double x, double y)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x * x + y * y);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double z = reinterpret(double, i - (exponent << 52));

    return kernel_atanhf_((z - 1) / (z + 1)) + ln2 / 2 * exponent;
}

static double carg_(double x, double y)
{
    const double pi = 3.14159265358979323846;

    return x < y ? pi / 2 - kernel_atanf_(x / y) : kernel_atanf_(y / x);
}

/* Complex inverse hyperbolic functions in the first quadrant
 *
 * cashf_(x, y, carg) = asinh(x + iy)
 * cashf_(x, y, atan2) = acosh(y + ix)
 */
static double _Complex cashf_(double x, double y, double arg(double, double))
{
    double xx = x * x;
    double yy = y * y;

    if (x < 0.5) {
        if (y < 1) {
            double s = xx * (2 + xx + 2 * yy);
            double t = 1 - yy;
            double u = sqrt(s + t * t) + t;
            double dx = sqrt(0.5 * (u + xx));
            double dy = x * y / dx;
            double re = 0.5 * log1pf_(xx + s / u + 2 * (x * dx + y * dy));
            double im = arg(x + dx, y + dy);

            return CMPLX(re, im);
        }
        if (y == 1) {
            double r = x * sqrt(4 + xx);
            double dx = sqrt(0.5 * (r + xx));
            double dy = sqrt(0.5 * (r - xx));
            double re = 0.5 * log1pf_(xx + r + 2 * (x * dx + dy));
            double im = arg(x + dx, 1 + dy);

            return CMPLX(re, im);
        }
    }

    double _Complex z = CMPLX(x, y) + csqrt_(xx - yy + 1, 2 * x * y);
    double re = z;
    double im = cimag(z);

    return CMPLX(real_clogf_(re, im), arg(re, im));
}
