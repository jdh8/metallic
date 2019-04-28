#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinhf(x) metallic_sinhf(x)
#include "src/math/float/sinhf.c"
#undef sinhf

BENCH(sinhf)
