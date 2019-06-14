#include "atan2f.h"
#include <complex.h>

float cargf(float _Complex z)
{
    return atan2f_(cimagf(z), z);
}
