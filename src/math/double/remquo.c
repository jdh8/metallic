#include "remquo.h"

double remquo(double numerator, double denominator, int quotient[static 1])
{
    return remquo_(numerator, denominator, quotient);
}
