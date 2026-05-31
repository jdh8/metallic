/* Exhaustive bit-for-bit cross-check of Metallic erff against CORE-MATH's
 * correctly-rounded cr_erff over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in erff.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define erff(x) metallic_erff(x)
#include "src/math/float/erff.c"
#undef erff

#include <binary32/erf/erff.c>   /* cr_erff */

#include "../../coremath.h"

static float fut(float x) { return metallic_erff(x); }
static float cr(float x) { return cr_erff(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
