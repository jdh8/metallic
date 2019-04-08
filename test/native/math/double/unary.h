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

#ifdef __SIZEOF_INT128__
static inline unsigned __int128 _bitcast(long double x)
{
    if (LDBL_MANT_DIG > 64)
        return reinterpret(unsigned __int128, x);

    return (union {
        long double from;
        unsigned __int128 to: 10;
    }){ x }.to;
}
#endif

static inline _Bool faithful(long double x, long double y)
{
    #if LDBL_MANT_DIG == DBL_MANT_DIG
        return approx(x, y, 1);
    #elif __SIZEOF_LONG_DOUBLE__ == __SIZEOF_INT128__
        const uint64_t mask = ((uint64_t)1 << (LDBL_MANT_DIG - DBL_MANT_DIG)) - 1;
        return _bitcast(x) - _bitcast(y) + mask <= 2 * mask;
    #else
        return identical(x, y) || (x && fabsl(x - y) <= fabs(nexttoward(x, y) - nexttoward(y, x)));
    #endif
}
