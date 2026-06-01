#include "common.h"
#include <math.h>
#include <stdio.h>

#define tgammaf(x) metallic_tgammaf(x)
#include "src/math/float/tgammaf.c"
#undef tgammaf

#ifdef BENCH_COREMATH
#include <binary32/tgamma/tgammaf.c>   /* cr_tgammaf */
BENCH_CR(tgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#else
BENCH(tgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#endif
