#include "common.h"
#include <math.h>
#include <stdio.h>

#define log1pf(x) metallic_log1pf(x)
#include "src/math/float/log1pf.c"
#undef log1pf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_log1pf, 29), bench(log1pf, 29));
}
