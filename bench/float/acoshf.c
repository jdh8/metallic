#include "common.h"
#include <math.h>
#include <stdio.h>

#define acoshf(x) metallic_acoshf(x)
#include "src/math/float/acoshf.c"
#undef acoshf

#ifdef BENCH_COREMATH
#include <binary32/acosh/acoshf.c>   /* cr_acoshf */
BENCH_CR(acoshf)
#else
BENCH(acoshf)
#endif
