/* Exhaustive bit-for-bit cross-check of Metallic logf against CORE-MATH's
 * correctly-rounded cr_logf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in logf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define logf(x) metallic_logf(x)
#include "src/math/float/logf.c"
#undef logf

#include <binary32/log/logf.c>   /* cr_logf */

#include "../../coremath.h"

static float fut(float x) { return metallic_logf(x); }
static float cr(float x) { return cr_logf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
