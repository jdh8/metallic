#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanf(x) metallic_atanf(x)
#include "src/math/float/atanf.c"
#undef atanf

BENCH(atanf)
