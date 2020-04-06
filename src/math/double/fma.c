#include "normalize.h"
#include "shift.h"
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

static double split_(double x)
{
    double s = (0x1p27 + 1) * x;
    double c = x - s;

    return s + c;
}

static double mul_(double a, double b, double r[static 1])
{
    double a0 = split_(a);
    double a1 = a - a0;
    double b0 = split_(b);
    double b1 = b - b0;

    double u = a0 * b0;
    double v = a0 * b1 + a1 * b0;
    double y = u + v;

    *r = u - y + v + a1 * b1;
    return y;
}

static double add_(double a, double b, double r[static 1])
{
    double y = a + b;
    double s = y - a;

    *r = s - y + a + (b - s);
    return y;
}

static double add_rounded_to_odd_(double a, double b)
{
    double e;
    double s = add_(a, b, &e);
    uint64_t i = reinterpret(uint64_t, s);

    if (e && !(i & 1)) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, e)) >> 62);
        return reinterpret(double, j);
    }
    return s;
}

static double add_subnormal_(double a, double b, int scale)
{
    double e;
    double s = add_(a, b, &e);
    uint64_t i = reinterpret(uint64_t, s);

    if (e && 1 - scale - (i << 1 >> 53) != (1 ^ (i & 1))) {
        uint64_t j = i + 1 - ((i ^ reinterpret(uint64_t, e)) >> 63 << 1);
        s = reinterpret(double, j);
    }
    return scalbn(s, scale);
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

    fesetround(FE_TONEAREST);

    double r;
    double ab = mul_(asig, bsig, &r);

    double e;
    double s = add_(ab, scale >= -106 ? shift_(csig, scale) : copysign(reinterpret(double, (uint64_t)1), csig), &e);

    fesetround(rounding);

    if (!s)
        return ab + csig + scalbn(r, exp);

    if (rounding != FE_TONEAREST)
        return scalbn(r + e + s, exp);

    double adjusted = add_rounded_to_odd_(r, e);

    if (exp + ilogb(s) > -1023)
        return scalbn(s + adjusted, exp);

    return add_subnormal_(s, adjusted, exp);
}
