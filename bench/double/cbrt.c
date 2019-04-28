#include "common.h"
#include <math.h>
#include <stdio.h>

#define cbrt(x) metallic_cbrt(x)
#include "src/math/double/cbrt.c"
#undef cbrt

BENCH(cbrt)
