#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanhf(x) metallic_atanhf(x)
#include "src/math/float/atanhf.c"
#undef atanhf

#ifdef BENCH_COREMATH
#include <binary32/atanh/atanhf.c>   /* cr_atanhf */
BENCH_CR(atanhf)
#else
BENCH(atanhf)
#endif
