#include "src/math/float/rem_pio2f.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define tanf(x) metallic_tanf(x)
#include "src/math/float/tanf.c"
#undef tanf

BENCH(tanf)
