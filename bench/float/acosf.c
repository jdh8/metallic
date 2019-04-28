#include "common.h"
#include <math.h>
#include <stdio.h>

#define acosf(x) metallic_acosf(x)
#include "src/math/float/acosf.c"
#undef acosf

BENCH(acosf)
