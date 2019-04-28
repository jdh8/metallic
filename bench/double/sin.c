#include "src/math/double/rem_pio2.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define sin(x) metallic_sin(x)
#include "src/math/double/sin.c"
#undef sin

BENCH(sin)
