#include "common.h"
#include <math.h>
#include <stdio.h>

#define exp(x) metallic_exp(x)
#include "src/math/double/exp.c"
#undef exp

BENCH(exp)
