#include "common.h"
#include <math.h>
#include <stdio.h>

#define tgammaf(x) metallic_tgammaf(x)
#include "src/math/float/tgammaf.c"
#undef tgammaf

BENCH(tgammaf)
