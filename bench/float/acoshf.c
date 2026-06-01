#include "common.h"
#include <math.h>
#include <stdio.h>

#define acoshf(x) metallic_acoshf(x)
#include "src/math/float/acoshf.c"
#undef acoshf

#ifdef BENCH_COREMATH
#include <binary32/acosh/acoshf.c>   /* cr_acoshf */
BENCH_CR(acoshf, BENCH_FROM(1.0f))
#else
BENCH(acoshf, BENCH_FROM(1.0f))
#endif
