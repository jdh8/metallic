#include "common.h"
#include <math.h>
#include <stdio.h>

#define logf(x) metallic_logf(x)
#include "src/math/float/logf.c"
#undef logf

NOINLINE float metallic_logf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_logf, 29), bench(logf, 29));
}
