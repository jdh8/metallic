#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

/* nexttoward(from, to) — like nextafter, but the direction is given in
 * long double so it can distinguish neighbors smaller than the double
 * ulp at `from`. We therefore compare in long double rather than
 * casting `to` down to double first.
 */
double nexttoward(double from, long double to)
{
    if (to != to)
        return (double)to;

    if (from != from)
        return from;

    long double lfrom = (long double)from;

    if (lfrom == to)
        return (double)to;

    if (from == 0)
        return __builtin_copysign(DBL_TRUE_MIN, (double)to);

    int64_t a = reinterpret(int64_t, from);

    /* Step `from` one ulp toward `to`. Increasing the bit pattern of a
     * positive double moves it away from zero; for negatives the sign
     * bit inverts the ordering — handled by the XOR test.
     */
    _Bool toward_zero = (from > 0 && to < lfrom) || (from < 0 && to > lfrom);
    return reinterpret(double, toward_zero ? a - 1 : a + 1);
}
