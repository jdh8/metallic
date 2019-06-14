#include "csinhf.h"

float _Complex csinf(float _Complex z)
{
    z = csinhf_(cimagf(z), z);

    return CMPLXF(cimagf(z), z);
}
