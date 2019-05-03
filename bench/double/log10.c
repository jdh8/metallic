#include "common.h"
#include <math.h>
#include <stdio.h>

#define log10(x) metallic_log10(x)
#include "src/math/double/log10.c"
#undef log10

BENCH(log10)
