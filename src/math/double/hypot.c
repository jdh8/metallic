#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double trunc_(double x)
{
    uint64_t bits = reinterpret(uint64_t, x) & 0xFFFFFFFF00000000;

    return reinterpret(double, bits);
}

static double safe_(double x, double y)
{
    double d = x - y;

    if (d > y) {
        double x1 = trunc_(x);
        double x2 = x - x1;
        return sqrt(y * y + x2 * (x + x1) + x1 * x1);
    }

    double t = 2 * x;
    double t1 = trunc_(t);
    double t2 = t - t1;
    double y1 = trunc_(y);
    double y2 = y - y1;

    return sqrt(t1 * y2 + t2 * y + d * d + t1 * y1);
}

static double ordered_(uint64_t a, uint64_t b)
{
    const uint64_t inf = 0x7FF0000000000000;

    double x = reinterpret(double, a);
    double y = reinterpret(double, b);

    if (a >= inf)
        return b == inf ? y : x;

    if (a - b >= 0x03C0000000000000) /* x / y >= 0x1p60 */
        return x + y;

    if (a > 0x5FD0000000000000) /* x > 0x1p+510 */
        return 0x1p+700 * safe_(0x1p-700 * x, 0x1p-700 * y);

    if (b < 0x23D0000000000000) /* y < 0x1p-450 */
        return 0x1p-700 * safe_(0x1p+700 * x, 0x1p+700 * y);

    return safe_(x, y);
}

double hypot(double x, double y)
{
    uint64_t a = reinterpret(uint64_t, fabs(x));
    uint64_t b = reinterpret(uint64_t, fabs(y));

    if (a < b)
        return ordered_(b, a);

    return ordered_(a, b);
}
