#ifndef BENCH_WASM  /* on wasm, metallic.a already defines __rem_pio2f */
#include "src/math/float/rem_pio2f.c"
#endif
#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinf(x) metallic_sinf(x)
#include "src/math/float/sinf.c"
#undef sinf

#ifdef BENCH_COREMATH
#include <binary32/sin/sinf.c>   /* cr_sinf */
BENCH_CR(sinf, BENCH_FULL)
#else
BENCH(sinf, BENCH_FULL)
#endif
