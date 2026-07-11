/* Exhaustive bit-for-bit cross-check of Metallic sinpif against CORE-MATH's
 * correctly-rounded cr_sinpif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define sinpif(x) metallic_sinpif(x)
#include "src/math/float/sinpif.c"
#undef sinpif

#include <binary32/sinpi/sinpif.c>   /* cr_sinpif */

#include "../../coremath.h"

static float fut(float x) { return metallic_sinpif(x); }
static float cr(float x) { return cr_sinpif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
