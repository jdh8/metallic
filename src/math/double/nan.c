#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

#include "../../stdlib/parse/nan.h"

double nan(const char s[static 1])
{
    char* dummy;
    return _nan(s, &dummy, '\0');
}
