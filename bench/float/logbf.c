#include "common.h"
#include <math.h>
#include <stdio.h>

#define logbf(x) metallic_logbf(x)
#include "src/math/float/logbf.c"
#undef logbf

BENCH(logbf)
