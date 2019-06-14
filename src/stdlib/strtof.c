#include "parse/scientificf.h"
#include <stdint.h>

typedef float Scalar;
typedef uint32_t Bitset;

#include "parse/float.h"

float strtof(const char s[restrict static 1], char** restrict end)
{
    return parsefloat_(s, end);
}
