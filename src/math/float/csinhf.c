#include "csinhf.h"

float _Complex csinhf(float _Complex z)
{
    return csinhf_(z, cimagf(z));
}
