/* Exhaustive bit-for-bit cross-check of Metallic tanpif against CORE-MATH's
 * correctly-rounded cr_tanpif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define tanpif(x) metallic_tanpif(x)
#include "src/math/float/tanpif.c"
#undef tanpif

#include <binary32/tanpi/tanpif.c>   /* cr_tanpif */

#include "../../coremath.h"

static float fut(float x) { return metallic_tanpif(x); }
static float cr(float x) { return cr_tanpif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
