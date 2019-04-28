#include "common.h"
#include <math.h>
#include <stdio.h>

#define expm1(x) metallic_expm1(x)
#include "src/math/double/expm1.c"
#undef expm1

BENCH(expm1)
