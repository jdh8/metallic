#include "../reinterpret.h"
#include <fenv.h>
#include <stdint.h>

static float fmaf_(float a, float b, float c)
{
    #pragma STDC FENV_ACCESS ON
    #pragma STDC FP_CONTRACT ON

    #ifdef FP_FAST_FMAF
        return a * b + c;
    #endif

    double ab = (double)a * b;
    double s = ab + c;
    uint64_t i = reinterpret(uint64_t, s);

    if ((i & 0x1FFFFFFF) != 0x10000000 || s - s || s - ab == c || fegetround() != FE_TONEAREST)
        return s;

    uint64_t adjustment = 1 - ((i ^ reinterpret(uint64_t, ab - s + c)) >> 63 << 1);
    return reinterpret(double, i + adjustment);
}
