#include "common.h"
#include <math.h>
#include <stdio.h>

#define expf(x) metallic_expf(x)
#include "src/math/float/expf.c"
#undef expf

#ifdef BENCH_COREMATH
#include <binary32/exp/expf.c>   /* cr_expf */
BENCH_CR(expf)
#else
BENCH(expf)
#endif
