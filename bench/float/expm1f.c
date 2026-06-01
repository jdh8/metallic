#include "common.h"
#include <math.h>
#include <stdio.h>

#define expm1f(x) metallic_expm1f(x)
#include "src/math/float/expm1f.c"
#undef expm1f

#ifdef BENCH_COREMATH
#include <binary32/expm1/expm1f.c>   /* cr_expm1f */
BENCH_CR(expm1f, BENCH_BOUNDED(-20.0f, 90.0f))
#else
BENCH(expm1f, BENCH_BOUNDED(-20.0f, 90.0f))
#endif
