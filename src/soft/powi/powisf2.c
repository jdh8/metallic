typedef float Scalar;

#include "powi.h"

float __powisf2(float x, int i)
{
    return powi_(x, i);
}
