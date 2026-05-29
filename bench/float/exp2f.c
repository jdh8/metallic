#include "common.h"
#include <math.h>
#include <stdio.h>

#define exp2f(x) metallic_exp2f(x)
#include "src/math/float/exp2f.c"
#undef exp2f

#ifdef BENCH_COREMATH
#include <binary32/exp2/exp2f.c>   /* cr_exp2f */
BENCH_CR(exp2f)
#else
BENCH(exp2f)
#endif
