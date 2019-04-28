#include "common.h"
#include <math.h>
#include <stdio.h>

#define cbrtf(x) metallic_cbrtf(x)
#include "src/math/float/cbrtf.c"
#undef cbrtf

BENCH(cbrtf)
