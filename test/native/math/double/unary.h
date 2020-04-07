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

static inline _Bool approx(long double x, long double y, long double tolerance)
{
    return identical(x, y) || fabsl(y - x) < tolerance * fabs(nexttoward(x, y) - (double)x);
}
