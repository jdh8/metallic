#include "common.h"
#include <math.h>
#include <stdio.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

#ifdef BENCH_WASM
/* CORE-MATH's lgammaf writes the legacy signgam global; metallic omits it (and
 * there is no system libm on wasm to supply it).  On native, libm provides it. */
int signgam;
#endif

#ifdef BENCH_COREMATH
#include <binary32/lgamma/lgammaf.c>   /* cr_lgammaf */
BENCH_CR(lgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#else
BENCH(lgammaf, BENCH_BOUNDED(-10.0f, 35.0f))
#endif
