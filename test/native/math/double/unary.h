#include "src/math/reinterpret.h"
#include "assertf.h"
#include <math.h>
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
    if (sizeof(long double) == sizeof(uint64_t))
        return approx(x, y, 1);

    return identical(x, y) || fabsl(y - x) < fabs(nexttoward(x, y) - (double)x);
}
