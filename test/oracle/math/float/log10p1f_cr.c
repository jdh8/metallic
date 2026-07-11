/* Exhaustive bit-for-bit cross-check of Metallic log10p1f against CORE-MATH's
 * correctly-rounded cr_log10p1f over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define log10p1f(x) metallic_log10p1f(x)
#include "src/math/float/log10p1f.c"
#undef log10p1f

#include <binary32/log10p1/log10p1f.c>   /* cr_log10p1f */

#include "../../coremath.h"

static float fut(float x) { return metallic_log10p1f(x); }
static float cr(float x) { return cr_log10p1f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
