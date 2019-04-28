#include "common.h"
#include <math.h>
#include <stdio.h>

#define acoshf(x) metallic_acoshf(x)
#include "src/math/float/acoshf.c"
#undef acoshf

BENCH(acoshf)
