/* Exhaustive bit-for-bit cross-check of Metallic lgammaf against CORE-MATH's
 * correctly-rounded cr_lgammaf over all 2^32 binary32 inputs.  Both claim
 * correct rounding, so they must agree everywhere. */

/* cr_lgammaf writes the POSIX `signgam` global. */
#define _GNU_SOURCE
#include <math.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

#include <binary32/lgamma/lgammaf.c>   /* cr_lgammaf */

#include "../../coremath.h"

static float fut(float x) { return metallic_lgammaf(x); }
static float cr(float x) { return cr_lgammaf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
