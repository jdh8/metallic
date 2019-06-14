#include "ccoshf.h"

float _Complex ccosf(float _Complex z)
{
    return ccoshf_(-cimagf(z), z);
}
