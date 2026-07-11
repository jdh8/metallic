/* Exhaustive bit-for-bit cross-check of Metallic cospif against CORE-MATH's
 * correctly-rounded cr_cospif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define cospif(x) metallic_cospif(x)
#include "src/math/float/cospif.c"
#undef cospif

#include <binary32/cospi/cospif.c>   /* cr_cospif */

#include "../../coremath.h"

static float fut(float x) { return metallic_cospif(x); }
static float cr(float x) { return cr_cospif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
