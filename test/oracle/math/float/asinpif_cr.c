/* Exhaustive bit-for-bit cross-check of Metallic asinpif against CORE-MATH's
 * correctly-rounded cr_asinpif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define asinpif(x) metallic_asinpif(x)
#include "src/math/float/asinpif.c"
#undef asinpif

#include <binary32/asinpi/asinpif.c>   /* cr_asinpif */

#include "../../coremath.h"

static float fut(float x) { return metallic_asinpif(x); }
static float cr(float x) { return cr_asinpif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
