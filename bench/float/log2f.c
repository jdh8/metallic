#include "common.h"
#include <math.h>
#include <stdio.h>

#define log2f(x) metallic_log2f(x)
#include "src/math/float/log2f.c"
#undef log2f

#ifdef BENCH_COREMATH
#include <binary32/log2/log2f.c>   /* cr_log2f */
BENCH_CR(log2f, BENCH_FROM(0.0f))
#else
BENCH(log2f, BENCH_FROM(0.0f))
#endif
