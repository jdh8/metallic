#include "common.h"
#include <math.h>
#include <stdio.h>

#define log1pf(x) metallic_log1pf(x)
#include "src/math/float/log1pf.c"
#undef log1pf

#ifdef BENCH_COREMATH
#include <binary32/log1p/log1pf.c>   /* cr_log1pf */
BENCH_CR(log1pf, BENCH_FROM(-1.0f))
#else
BENCH(log1pf, BENCH_FROM(-1.0f))
#endif
