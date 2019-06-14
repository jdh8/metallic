#include <stdint.h>

typedef float Scalar;
typedef uint32_t Bitset;

#include "../../stdlib/parse/nan.h"

float nanf(const char s[static 1])
{
    char* dummy;
    return nan_(s, &dummy, '\0');
}
