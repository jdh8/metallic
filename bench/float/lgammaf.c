#include "common.h"
#include <math.h>
#include <stdio.h>

#define lgammaf(x) metallic_lgammaf(x)
#include "src/math/float/lgammaf.c"
#undef lgammaf

NOINLINE float metallic_lgammaf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_lgammaf, 29), bench(lgammaf, 29));
}
