#include "common.h"
#include <math.h>
#include <stdio.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

#ifdef BENCH_COREMATH
#include <binary32/lgamma/lgammaf.c>   /* cr_lgammaf */
BENCH_CR(lgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#else
BENCH(lgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#endif
