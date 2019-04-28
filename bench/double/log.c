#include "common.h"
#include <math.h>
#include <stdio.h>

#define log(x) metallic_log(x)
#include "src/math/double/log.c"
#undef log

BENCH(log)
