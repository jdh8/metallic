#include "common.h"
#include <math.h>
#include <stdio.h>

#define erff(x) metallic_erff(x)
#include "src/math/float/erff.c"
#undef erff

BENCH(erff)
