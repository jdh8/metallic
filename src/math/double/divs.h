#include "truncate.h"

/* Faithfully rounded c / (a + b) where |a| > |b| or |a| is a power of 2 */
static double divs_(double c, double a, double b)
{
    double s = truncate_(a + b, 32);
    double x = truncate_(c / (a + b), 21);

    return x + (c - x * s - x * (a - s + b)) / (a + b);
}
