#include "common.h"
#include <math.h>
#include <stdio.h>

#define expf(x) metallic_expf(x)
#include "src/math/float/expf.c"
#undef expf

#ifdef BENCH_COREMATH
#include <binary32/exp/expf.c>   /* cr_expf */
BENCH_CR(expf, BENCH_BOUNDED(-105.0f, 90.0f))
#else
BENCH(expf, BENCH_BOUNDED(-105.0f, 90.0f))
#endif
