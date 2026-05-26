#include "parse/scientificl.h"
#include <stdint.h>

typedef long double Scalar;
typedef unsigned __int128 Bitset;

#include "parse/float.h"

long double strtold(const char s[restrict static 1], char** restrict end)
{
    return parsefloat_(s, end);
}
