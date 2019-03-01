#include "ccoshf.h"

float _Complex ccosf(float _Complex z)
{
    return _ccoshf(-cimagf(z), z);
}
