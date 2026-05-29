#include "common.h"
#include <math.h>
#include <stdio.h>

#define cbrtf(x) metallic_cbrtf(x)
#include "src/math/float/cbrtf.c"
#undef cbrtf

#ifdef BENCH_COREMATH
#include <binary32/cbrt/cbrtf.c>   /* cr_cbrtf */
BENCH_CR(cbrtf)
#else
BENCH(cbrtf)
#endif
