#include "common.h"
#include <math.h>
#include <stdio.h>

#define exp2f(x) metallic_exp2f(x)
#include "src/math/float/exp2f.c"
#undef exp2f

BENCH(exp2f)
