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
    double s = (0x1p27 + 1) * x;
    double c = x - s;

    y[1] = s + c;
    y[0] = x - y[1];
}

static void mul_(double y[static 2], double a, double b)
{
    double u[2], v[2];

    split_(u, a);
    split_(v, b);

    double p = u[1] * v[1];
    double q = u[1] * v[0] + u[0] + v[1];

    y[1] = p + q;
    y[0] = p - y[1] + q + u[0] * v[0];
}

static void add_(double y[static 2], double a, double b)
{
    y[1] = a + b;

    double s = y[1] - a;

    y[0] = s - y[1] + a + (b - s);
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

double fma(double a, double b, double c)
{
    if (a - a || b - b)
        return a * b + c;

    if (c - c)
        return c;

    if (!a || !b)
        return a * b + c;

    if (!c)
        return a * b;

    int aexp, bexp, cexp;
    double asig = frexp_(a, &aexp);
    double bsig = frexp_(b, &bexp);
    double csig = frexp_(c, &cexp);

    int exp = aexp + bexp;
    int scale = cexp - exp;

    if (scale > 53)
        return c;

    double ab[2], s[2];

    mul_(ab, asig, bsig);
    add_(s, ab[1], scale >= -106 ? scalbn_(csig, scale) : copysign(reinterpret(double, (uint64_t)1), csig));

    if (!s[1])
        return ab[1] + csig + scalbn(ab[0], exp);

    double adjusted = add_rounded_to_odd_(ab[0], s[0]);

    if (exp + ilogb(s[1]) > -1023)
        return scalbn(s[1] + adjusted, exp);

    return add_subnormal_(s[1], adjusted, exp);
}
