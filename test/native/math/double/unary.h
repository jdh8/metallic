#include "src/math/reinterpret.h"
#include "assertf.h"
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <stdio.h>

#define verify(cond, x) ASSERTF(cond, "at %a\n", (double)(x))

static inline _Bool identical(double x, double y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

static inline _Bool approx(double x, double y, uint64_t tolerance)
{
    uint64_t a = reinterpret(uint64_t, x);
    uint64_t b = reinterpret(uint64_t, y);

    return a - b + tolerance <= 2 * tolerance;
}

static inline _Bool faithful(long double x, long double y)
{
#if LDBL_MANT_DIG > DBL_MANT_DIG
    const uint64_t mask = ((uint64_t)1 << (LDBL_MANT_DIG - DBL_MANT_DIG)) - 1;

    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return a - b + mask <= 2 * mask;
#else
    return approx(x, y, 1);
#endif
}
