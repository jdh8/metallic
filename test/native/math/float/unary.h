#include "src/math/reinterpret.h"
#include "assertf.h"
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>

#define verify(cond, x) ASSERTF(cond, "at %a\n", (double)(x))

static inline _Bool identical(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

static inline _Bool faithful(double x, double y)
{
    return identical(x, y) || fabs(y - x) < fabsf(nexttowardf(x, y) - (float)x);
}
