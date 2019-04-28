#include "common.h"
#include <math.h>
#include <stdio.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

BENCH(lgammaf)
