#include "../reinterpret.h"
#include <fenv.h>
#include <stdint.h>

float fmaf(float a, float b, float c)
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

    #ifdef FE_TOWARDZERO
        uint64_t adjustment = !fesetround(FE_TOWARDZERO) && c + 0 + ab == s;
        fesetround(FE_TONEAREST);
    #else
        uint64_t adjustment = 1 - ((i ^ reinterpret(uint64_t, s - ab - c)) >> 63 << 1);
    #endif

    return reinterpret(double, i + adjustment);
}
