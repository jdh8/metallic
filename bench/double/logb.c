#include "common.h"
#include <math.h>
#include <stdio.h>

#define logb(x) metallic_logb(x)
#include "src/math/double/logb.c"
#undef logb

BENCH(logb)
