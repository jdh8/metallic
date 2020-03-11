#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double frexp_(double x, int exp[static 1])
{
    int64_t i = normalize_(reinterpret(int64_t, fabs(x)));

    *exp = (i >> 52) - 1022;

    return copysign(reinterpret(double, (i & 0x000FFFFFFFFFFFFF) | 0x3FE0000000000000), x);
}

static double scalbn_(double x, int exp)
{
    int64_t i = reinterpret(int64_t, x) + ((int64_t)exp << 52);

    return reinterpret(double, i);
}

static void split_(double y[static 2], double x)
{
    double c = (0x1p27 + 1) * x;
    double s = x - c;

    y[1] = s + c;
    y[0] = x - y[1];
}

static void mul_(double z[static 2], double a, double b)
{
    double x[2], y[2];

    split_(x, a);
    split_(y, b);

    double p = x[1] * y[1];
    double q = x[1] * y[0] + x[0] + y[1];

    z[1] = p + q;
    z[0] = p - z[1] + q + x[0] * y[0];
}

static void add_(double z[static 2], double a, double b)
{
    z[1] = a + b;

    double s = z[1] - a;

    z[0] = s - z[1] + a + (b - s);
}

static double add_rounded_to_odd_(double a, double b)
{
    double sum[2];

    add_(sum, a, b);

    uint64_t i = reinterpret(uint64_t, sum[1]);

    if (sum[0] && !(i & 1)) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, sum[0])) >> 62);
        return reinterpret(double, j);
    }
    return sum[1];
}

static double add_subnormal_(double a, double b, int scale)
{
    double sum[2];

    add_(sum, a, b);

    uint64_t i = reinterpret(uint64_t, sum[1]);

    if (sum[0] && 1 - scale - (i << 1 >> 53) != (1 ^ (i & 1))) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, sum[0])) >> 63 << 1);
        sum[1] = reinterpret(double, j);
    }
    return scalbn(sum[1], scale);
}

double fma(double x, double y, double a)
{
    if (x - x || y - y)
        return x * y + a;

    if (a - a)
        return a;

    if (!x || !y)
        return x * y + a;

    if (!a)
        return x * y;

    int xe, ye, ae;
    double xs = frexp_(x, &xe);
    double ys = frexp_(y, &ye);
    double as = frexp_(a, &ae);

    int exp = xe + ye;
    int scale = ae - exp;

    if (scale > 53)
        return a;

    double xy[2], r[2];

    mul_(xy, xs, ys);
    add_(r, xy[1], scale >= -106 ? scalbn_(as, exp) : copysign(reinterpret(double, (uint64_t)1), as));

    if (!r[1])
        return xy[1] + as + scalbn(xy[0], exp);

    double adjusted = add_rounded_to_odd_(xy[0], r[0]);

    if (exp + ilogb(r[1]) > -1023)
        return scalbn(r[1] + adjusted, exp);

    return add_subnormal_(r[1], adjusted, exp);
}
