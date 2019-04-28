#include "src/math/double/rem_pio2.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define cos(x) metallic_cos(x)
#include "src/math/double/cos.c"
#undef cos

BENCH(cos)
