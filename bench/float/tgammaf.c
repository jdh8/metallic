#include "common.h"
#include <math.h>
#include <stdio.h>

#define tgammaf(x) metallic_tgammaf(x)
#include "src/math/float/tgammaf.c"
#undef tgammaf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_tgammaf, 29), bench(tgammaf, 29));
}
