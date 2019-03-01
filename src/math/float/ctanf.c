#include <complex.h>

float _Complex ctanf(float _Complex z)
{
    z = ctanhf(CMPLXF(cimagf(z), z));

    return CMPLXF(cimagf(z), z);
}
