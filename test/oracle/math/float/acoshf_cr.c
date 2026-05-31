/* Exhaustive bit-for-bit cross-check of Metallic acoshf against CORE-MATH's
 * correctly-rounded cr_acoshf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in acoshf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define acoshf(x) metallic_acoshf(x)
#include "src/math/float/acoshf.c"
#undef acoshf

#include <binary32/acosh/acoshf.c>   /* cr_acoshf */

#include "../../coremath.h"

static float fut(float x) { return metallic_acoshf(x); }
static float cr(float x) { return cr_acoshf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
