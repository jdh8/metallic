#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinhf(x) metallic_asinhf(x)
#include "src/math/float/asinhf.c"
#undef asinhf

BENCH(asinhf)
