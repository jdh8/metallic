#include "common.h"
#include <math.h>
#include <stdio.h>

#define tanhf(x) metallic_tanhf(x)
#include "src/math/float/tanhf.c"
#undef tanhf

BENCH(tanhf)
