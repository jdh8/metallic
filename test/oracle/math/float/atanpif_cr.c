/* Exhaustive bit-for-bit cross-check of Metallic atanpif against CORE-MATH's
 * correctly-rounded cr_atanpif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define atanpif(x) metallic_atanpif(x)
#include "src/math/float/atanpif.c"
#undef atanpif

#include <binary32/atanpi/atanpif.c>   /* cr_atanpif */

#include "../../coremath.h"

static float fut(float x) { return metallic_atanpif(x); }
static float cr(float x) { return cr_atanpif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
