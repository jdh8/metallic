#include "ccoshf.h"

float _Complex ccoshf(float _Complex z)
{
    return _ccoshf(z, cimagf(z));
}
