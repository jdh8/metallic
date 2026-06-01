#include "common.h"
#include <math.h>
#include <stdio.h>

#define acosf(x) metallic_acosf(x)
#include "src/math/float/acosf.c"
#undef acosf

#ifdef BENCH_COREMATH
#include <binary32/acos/acosf.c>   /* cr_acosf */
BENCH_CR(acosf, BENCH_BOUNDED(-1.1f, 1.1f))
#else
BENCH(acosf, BENCH_BOUNDED(-1.1f, 1.1f))
#endif
