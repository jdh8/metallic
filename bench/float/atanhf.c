#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanhf(x) metallic_atanhf(x)
#include "src/math/float/atanhf.c"
#undef atanhf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_atanhf), bench(atanhf));
}
