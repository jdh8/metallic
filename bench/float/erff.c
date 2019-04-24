#include "common.h"
#include <math.h>
#include <stdio.h>

#define erff(x) metallic_erff(x)
#include "src/math/float/erff.c"
#undef erff

NOINLINE float metallic_erff(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_erff, 29), bench(erff, 29));
}
