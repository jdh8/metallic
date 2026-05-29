#include "common.h"
#include <math.h>
#include <stdio.h>

#define log10f(x) metallic_log10f(x)
#include "src/math/float/log10f.c"
#undef log10f

#ifdef BENCH_COREMATH
#include <binary32/log10/log10f.c>   /* cr_log10f */
BENCH_CR(log10f)
#else
BENCH(log10f)
#endif
