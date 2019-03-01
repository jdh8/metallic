#include "csinhf.h"

float _Complex csinhf(float _Complex z)
{
    return _csinhf(z, cimagf(z));
}
