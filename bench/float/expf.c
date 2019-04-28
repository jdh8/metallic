#include "common.h"
#include <math.h>
#include <stdio.h>

#define expf(x) metallic_expf(x)
#include "src/math/float/expf.c"
#undef expf

BENCH(expf)
