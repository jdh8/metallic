#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinhf(x) metallic_asinhf(x)
#include "src/math/float/asinhf.c"
#undef asinhf

#ifdef BENCH_COREMATH
#include <binary32/asinh/asinhf.c>   /* cr_asinhf */
BENCH_CR(asinhf)
#else
BENCH(asinhf)
#endif
