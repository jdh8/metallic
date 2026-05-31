#include "common.h"
#include <math.h>
#include <stdio.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

#ifdef BENCH_COREMATH
#include <binary32/lgamma/lgammaf.c>   /* cr_lgammaf */
BENCH_CR(lgammaf)
#else
BENCH(lgammaf)
#endif
