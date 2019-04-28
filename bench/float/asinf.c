#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinf(x) metallic_asinf(x)
#include "src/math/float/asinf.c"
#undef asinf

BENCH(asinf)
