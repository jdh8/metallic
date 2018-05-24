#include <math.h>
#include <time.h>
#include <stdio.h>

float f(float);
float f0(float);

int bench(int repeats, float a, float b)
{
    const double resolution = 1.0 / CLOCKS_PER_SEC;

    float x[repeats], y[repeats], y0[repeats];

    for (int k = 0; k < repeats; ++k)
        x[k] = a + b * k / repeats;

    clock_t tick = clock();

    for (int k = 0; k < repeats; ++k)
        y[k] = f(x[k]);

    printf("f: %g\n", (clock() - tick) * resolution);

    tick = clock();

    for (int k = 0; k < repeats; ++k)
        y0[k] = f0(x[k]);

    printf("f0: %g\n", (clock() - tick) * resolution);

    double norm = -0.0;

    for (int k = 0; k < repeats; ++k) {
        double diff = y[k] - y0[k];
        norm += diff * diff;
    }

    printf("\nError norm: %g\n", sqrt(norm));
}
