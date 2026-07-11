/* Exhaustive bit-for-bit cross-check of Metallic acospif against CORE-MATH's
 * correctly-rounded cr_acospif over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define acospif(x) metallic_acospif(x)
#include "src/math/float/acospif.c"
#undef acospif

#include <binary32/acospi/acospif.c>   /* cr_acospif */

#include "../../coremath.h"

static float fut(float x) { return metallic_acospif(x); }
static float cr(float x) { return cr_acospif(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
