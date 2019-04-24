#include "common.h"
#include <math.h>
#include <stdio.h>

#define log10f(x) metallic_log10f(x)
#include "src/math/float/log10f.c"
#undef log10f

NOINLINE float metallic_log10f(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_log10f, 29), bench(log10f, 29));
}
