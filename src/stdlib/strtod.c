#include "parse/scientific.h"
#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

#include "parse/float.h"

double strtod(const char s[restrict static 1], char** restrict end)
{
    return parsefloat_(s, end);
}
