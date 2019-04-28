#include "common.h"
#include <math.h>
#include <stdio.h>

#define log1pf(x) metallic_log1pf(x)
#include "src/math/float/log1pf.c"
#undef log1pf

BENCH(log1pf)
