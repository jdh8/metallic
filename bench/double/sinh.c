#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinh(x) metallic_sinh(x)
#include "src/math/double/sinh.c"
#undef sinh

BENCH(sinh)
