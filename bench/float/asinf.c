#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinf(x) metallic_asinf(x)
#include "src/math/float/asinf.c"
#undef asinf

#ifdef BENCH_COREMATH
#include <binary32/asin/asinf.c>   /* cr_asinf */
BENCH_CR(asinf)
#else
BENCH(asinf)
#endif
