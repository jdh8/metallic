#include "common.h"
#include <math.h>
#include <stdio.h>

#define log10f(x) metallic_log10f(x)
#include "src/math/float/log10f.c"
#undef log10f

BENCH(log10f)
