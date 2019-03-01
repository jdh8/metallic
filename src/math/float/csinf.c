#include "csinhf.h"

float _Complex csinf(float _Complex z)
{
    z = _csinhf(cimagf(z), z);

    return CMPLXF(cimagf(z), z);
}
