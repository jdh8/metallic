#include "atan2f.h"
#include <complex.h>

float cargf(float _Complex z)
{
    return _atan2f(cimagf(z), z);
}
