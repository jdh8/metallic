#include <math.h>

int __rem_pio2f(float x, double y[static 1]);

/* After __rem_pio2f reduces x to y in [-π/4, π/4] with quadrant q, tan and cot
 * share two degree-3 polynomials in u = y²: in an even quadrant
 * tan(y) = y·num(u)/den(u); in an odd quadrant we want
 * -cot(y) = -den(u)/(y·num(u)), the same polynomials with numerator and
 * denominator swapped.  The rational is accurate to ~2⁻⁵¹ relative, so plain
 * double Horner (no FMA — WASM has none) is far more than the ~25 bits a
 * correctly-rounded float needs, and the old hardcoded hard-to-round cases
 * drop out.
 *
 * Coefficients generated with rminimax/ratapprox:
 *   ratapprox --function="tan(sqrt(x))/sqrt(x)" --dom="[0.0001,0.6168]"
 *     --type=[3,3] --numF=D --denF=D
 */
float tanf(float x)
{
    const double num[] = {
        1.0,
       -0.12828240124149537,
        2.80579910541274e-3,
       -7.482480453622507e-6
    };
    const double den[] = {
        1.0,
       -0.46161573457482674,
        2.334437729696323e-2,
       -2.0843093714183495e-4
    };

    double y;
    unsigned q = __rem_pio2f(x, &y);
    double u = y * y;
    double p = ((num[3] * u + num[2]) * u + num[1]) * u + num[0];
    double d = ((den[3] * u + den[2]) * u + den[1]) * u + den[0];
    double yp = y * p;

    return q & 1 ? -d / yp : yp / d;
}
