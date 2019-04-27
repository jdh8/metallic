#include "common.h"
#include <math.h>
#include <stdio.h>

#define expm1f(x) metallic_expm1f(x)
#include "src/math/float/expm1f.c"
#undef expm1f

int main(void)
{
    printf("%f\n%f\n", bench(metallic_expm1f), bench(expm1f));
}
