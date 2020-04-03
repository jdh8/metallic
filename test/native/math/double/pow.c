#include "src/math/double/pow.c"
#include "binary.h"
#include <stdio.h>

static void swapped(double y, double x)
{
    verify2(faithful(pow(x, y), powl(x, y)), x, y);
}

static void unity(double y, double x)
{
    verify2(pow(x, y) == 1, x, y);
}

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t step = 0x000ABA55C128D4B5;

    for (uint64_t j = 0; j < inf; j += step)
        for (uint64_t i = 0; i < inf; i += step)
            mirror(swapped, reinterpret(double, j), reinterpret(double, i));

    for (uint64_t i = 0; i < 0x8000000000000000; i += step) {
        double any = reinterpret(double, i);
        mirror(unity, any, 1);
        mirror(unity, 0, any);
    }

    for (uint64_t i = 0; i < inf; i += step)
        for (int y = 0; y < 1024; ++y)
            quadrants(swapped, y, reinterpret(double, i));

    for (uint64_t i = 0; i < inf; i += step) {
        double x = reinterpret(double, i);
        quadrants(swapped, INFINITY, x);
        quadrants(swapped, x, INFINITY);
    }

    quadrants(swapped, INFINITY, INFINITY);
}
