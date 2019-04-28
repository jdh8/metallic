#include "src/math/float/rem_pio2f.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinf(x) metallic_sinf(x)
#include "src/math/float/sinf.c"
#undef sinf

BENCH(sinf)
