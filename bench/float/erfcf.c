#include "common.h"
#include <math.h>
#include <stdio.h>

#define erfcf(x) metallic_erfcf(x)
#include "src/math/float/erfcf.c"
#undef erfcf

BENCH(erfcf)
