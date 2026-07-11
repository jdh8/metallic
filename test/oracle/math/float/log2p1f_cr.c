/* Exhaustive bit-for-bit cross-check of Metallic log2p1f against CORE-MATH's
 * correctly-rounded cr_log2p1f over all 2^32 binary32 inputs. */

#define _GNU_SOURCE
#include <math.h>

#define log2p1f(x) metallic_log2p1f(x)
#include "src/math/float/log2p1f.c"
#undef log2p1f

#include <binary32/log2p1/log2p1f.c>   /* cr_log2p1f */

#include "../../coremath.h"

static float fut(float x) { return metallic_log2p1f(x); }
static float cr(float x) { return cr_log2p1f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
