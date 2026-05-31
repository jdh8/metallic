/* Exhaustive bit-for-bit cross-check of Metallic cosf against CORE-MATH's
 * correctly-rounded cr_cosf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in cosf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define cosf(x) metallic_cosf(x)
#include "src/math/float/cosf.c"
#undef cosf
#include "src/math/float/rem_pio2f.c"

#include <binary32/cos/cosf.c>   /* cr_cosf */

#include "../../coremath.h"

static float fut(float x) { return metallic_cosf(x); }
static float cr(float x) { return cr_cosf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
