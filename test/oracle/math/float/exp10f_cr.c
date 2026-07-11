/* Exhaustive bit-for-bit cross-check of Metallic exp10f against CORE-MATH's
 * correctly-rounded cr_exp10f over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define exp10f(x) metallic_exp10f(x)
#include "src/math/float/exp10f.c"
#undef exp10f

#include <binary32/exp10/exp10f.c>   /* cr_exp10f */

#include "../../coremath.h"

static float fut(float x) { return metallic_exp10f(x); }
static float cr(float x) { return cr_exp10f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
