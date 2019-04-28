#include "common.h"
#include <math.h>
#include <stdio.h>

#define exp2(x) metallic_exp2(x)
#include "src/math/double/exp2.c"
#undef exp2

BENCH(exp2)
