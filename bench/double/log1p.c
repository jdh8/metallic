#include "common.h"
#include <math.h>
#include <stdio.h>

#define log1p(x) metallic_log1p(x)
#include "src/math/double/log1p.c"
#undef log1p

BENCH(log1p)
