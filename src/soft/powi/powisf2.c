typedef float Scalar;

#include "powi.h"

float __powisf2(float x, int i)
{
    return _powi(x, i);
}
