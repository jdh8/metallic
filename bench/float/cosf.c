#ifndef BENCH_WASM  /* on wasm, metallic.a already defines __rem_pio2f */
#include "src/math/float/rem_pio2f.c"
#endif
#include "common.h"
#include <math.h>
#include <stdio.h>

#define cosf(x) metallic_cosf(x)
#include "src/math/float/cosf.c"
#undef cosf

#ifdef BENCH_COREMATH
#include <binary32/cos/cosf.c>   /* cr_cosf */
BENCH_CR(cosf, BENCH_FULL)
#else
BENCH(cosf, BENCH_FULL)
#endif
