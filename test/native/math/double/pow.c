#include "src/math/double/pow.c"
#include "binary.h"
#include <stdio.h>

static void swapped(double y, double x)
{
    verify2(approx(pow(x, y), powl(x, y), 1), x, y);
}

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t step = 0x000ABA55C128D4B5;

    for (uint64_t j = 0; j < inf; j += step)
        for (uint64_t i = 0; i < inf; i += step)
            mirror(swapped, reinterpret(double, j), reinterpret(double, i));
}

