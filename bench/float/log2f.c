#include "common.h"
#include <math.h>
#include <stdio.h>

#define log2f(x) metallic_log2f(x)
#include "src/math/float/log2f.c"
#undef log2f

BENCH(log2f)
