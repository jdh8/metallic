#include "common.h"
#include <math.h>
#include <stdio.h>

#define log2(x) metallic_log2(x)
#include "src/math/double/log2.c"
#undef log2

BENCH(log2)
