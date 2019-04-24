#include "common.h"
#include <math.h>
#include <stdio.h>

#define log2f(x) metallic_log2f(x)
#include "src/math/float/log2f.c"
#undef log2f

int main(void)
{
    printf("%f\n%f\n", bench(metallic_log2f, 29), bench(log2f, 29));
}
