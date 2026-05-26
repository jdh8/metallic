#include "parse/scientific.h"
#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

static double decimal_to_scalar_(const decimal_t* d)
{
    return decimal_to_double_(d);
}

#include "parse/float.h"

double strtod(const char s[restrict static 1], char** restrict end)
{
    return parsefloat_(s, end);
}
