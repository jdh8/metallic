#include "ccoshf.h"

float _Complex ccoshf(float _Complex z)
{
    return ccoshf_(z, cimagf(z));
}
