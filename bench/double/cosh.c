#include "common.h"
#include <math.h>
#include <stdio.h>

#define cosh(x) metallic_cosh(x)
#include "src/math/double/cosh.c"
#undef cosh

BENCH(cosh)
