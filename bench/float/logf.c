#include "common.h"
#include <math.h>
#include <stdio.h>

#define logf(x) metallic_logf(x)
#include "src/math/float/logf.c"
#undef logf

#ifdef BENCH_COREMATH
#include <binary32/log/logf.c>   /* cr_logf */
BENCH_CR(logf)
#else
BENCH(logf)
#endif
