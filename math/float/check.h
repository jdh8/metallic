#include <math.h>
#include <float.h>
#include <stdio.h>

float f(float);
double f0(float);

int check(int repeats, float a, float b)
{
    int status = 0;

    for (int k = 0; k < repeats; ++k) {
        float x = a + b * k / repeats;
        double y = f(x);
        double y0 = f0(x);
        double diff = fabs((y - y0) / y0);

        if (diff > FLT_EPSILON) {
            status = -1;
            printf("%g\t%g\t%g\t%g\n", x, y, y0, diff / FLT_EPSILON);
        }
    }

    return status;
}
