#include "common.h"
#include <math.h>
#include <stdio.h>

#define exp2f(x) metallic_exp2f(x)
#include "src/math/float/exp2f.c"
#undef exp2f

int main(void)
{
    printf("%f\n%f\n", bench(metallic_exp2f, 29), bench(exp2f, 29));
}
