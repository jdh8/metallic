#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>
#include <fenv.h>
#include <stdint.h>

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT ON

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

static double split_(double x)
{
    double s = (0x1p27 + 1) * x;
    double c = x - s;

    return s + c;
}

static void mul_(double y[static 2], double a, double b)
{
    double a0 = split_(a);
    double a1 = a - a0;
    double b0 = split_(b);
    double b1 = b - b0;

    double u = a0 * b0;
    double v = a0 * b1 + a1 * b0;

    y[0] = u + v;
    y[1] = u - y[0] + v + a1 * b1;
}

static void add_(double y[static 2], double a, double b)
{
    y[0] = a + b;

    double s = y[0] - a;

    y[1] = s - y[0] + a + (b - s);
}

static double add_rounded_to_odd_(double a, double b)
{
    double sum[2];

    add_(sum, a, b);

    uint64_t i = reinterpret(uint64_t, sum[0]);

    if (sum[1] && !(i & 1)) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, sum[1])) >> 62);
        return reinterpret(double, j);
    }
    return sum[0];
}

static double add_subnormal_(double a, double b, int scale)
{
    double sum[2];

    add_(sum, a, b);

    uint64_t i = reinterpret(uint64_t, sum[0]);

    if (sum[1] && 1 - scale - (i << 1 >> 53) != (1 ^ (i & 1))) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, sum[1])) >> 63 << 1);
        sum[0] = reinterpret(double, j);
    }
    return scalbn(sum[0], scale);
}

double fma(double a, double b, double c)
{
    #ifdef FP_FAST_FMA
        return a * b + c;
    #endif

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
    int rounding = fegetround();

    if (scale > 53) {
        uint64_t i = reinterpret(uint64_t, c);

        switch (rounding) {
            #ifdef FE_TOWARDZERO
                case FE_TOWARDZERO:
                    return reinterpret(double, i - (signbit(a) ^ signbit(b) ^ signbit(c)));
            #endif
            #ifdef FE_UPWARD
                case FE_UPWARD:
                    return reinterpret(double, i + (signbit(a) == signbit(b)) * (1 - 2 * signbit(c)));
            #endif
            #ifdef FE_DOWNWARD
                case FE_DOWNWARD:
                    return reinterpret(double, i - (signbit(a) != signbit(b)) * (1 - 2 * signbit(c)));
            #endif
        }
        return c;
    }

    double ab[2], s[2];

    fesetround(FE_TONEAREST);
    mul_(ab, asig, bsig);
    add_(s, ab[0], scale >= -106 ? scalbn_(csig, scale) : copysign(reinterpret(double, (uint64_t)1), csig));
    fesetround(rounding);

    if (!s[0])
        return ab[0] + csig + scalbn(ab[1], exp);

    if (rounding != FE_TONEAREST)
        return scalbn(ab[1] + s[1] + s[0], exp);

    double adjusted = add_rounded_to_odd_(ab[1], s[1]);

    if (exp + ilogb(s[0]) > -1023)
        return scalbn(s[0] + adjusted, exp);

    return add_subnormal_(s[0], adjusted, exp);
}
