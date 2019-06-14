typedef double Scalar;

#include "powi.h"

double __powidf2(double x, int i)
{
    return powi_(x, i);
}
