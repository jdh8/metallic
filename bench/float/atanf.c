#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanf(x) metallic_atanf(x)
#include "src/math/float/atanf.c"
#undef atanf

#ifdef BENCH_COREMATH
#include <binary32/atan/atanf.c>   /* cr_atanf */
BENCH_CR(atanf, BENCH_FULL)
#else
BENCH(atanf, BENCH_FULL)
#endif
