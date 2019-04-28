#include "src/math/double/rem_pio2.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define tan(x) metallic_tan(x)
#include "src/math/double/tan.c"
#undef tan

BENCH(tan)
