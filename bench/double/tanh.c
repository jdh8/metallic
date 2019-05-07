#include "common.h"
#include <math.h>
#include <stdio.h>

#define tanh(x) metallic_tanh(x)
#include "src/math/double/tanh.c"
#undef tanh

BENCH(tanh)
