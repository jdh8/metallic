#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanhf(x) metallic_atanhf(x)
#include "src/math/float/atanhf.c"
#undef atanhf

BENCH(atanhf)
