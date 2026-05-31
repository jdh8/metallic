/* Exhaustive bit-for-bit cross-check of Metallic tgammaf against CORE-MATH's
 * correctly-rounded cr_tgammaf over all 2^32 binary32 inputs.  Both claim
 * correct rounding, so they must agree everywhere. */

#define tgammaf(x) metallic_tgammaf(x)
#include "src/math/float/tgammaf.c"
#undef tgammaf

#include <binary32/tgamma/tgammaf.c>   /* cr_tgammaf */

#include "../../coremath.h"

static float fut(float x) { return metallic_tgammaf(x); }
static float cr(float x) { return cr_tgammaf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
