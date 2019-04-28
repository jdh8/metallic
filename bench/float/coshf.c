#include "common.h"
#include <math.h>
#include <stdio.h>

#define coshf(x) metallic_coshf(x)
#include "src/math/float/coshf.c"
#undef coshf

BENCH(coshf)
