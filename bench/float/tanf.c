#ifndef BENCH_WASM  /* on wasm, metallic.a already defines __rem_pio2f */
#include "src/math/float/rem_pio2f.c"
#endif
#include "common.h"
#include <math.h>
#include <stdio.h>

#define tanf(x) metallic_tanf(x)
#include "src/math/float/tanf.c"
#undef tanf

#ifdef BENCH_COREMATH
#include <binary32/tan/tanf.c>   /* cr_tanf */
BENCH_CR(tanf, BENCH_FULL)
#else
BENCH(tanf, BENCH_FULL)
#endif
