/* Exhaustive bit-for-bit cross-check of Metallic log1pf against CORE-MATH's
 * correctly-rounded cr_log1pf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in log1pf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define log1pf(x) metallic_log1pf(x)
#include "src/math/float/log1pf.c"
#undef log1pf

#include <binary32/log1p/log1pf.c>   /* cr_log1pf */

#include "../../coremath.h"

static float fut(float x) { return metallic_log1pf(x); }
static float cr(float x) { return cr_log1pf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
