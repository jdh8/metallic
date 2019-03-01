#include <stdlib.h>

#define CAT2(a, b) a##b
#define CAT(a, b)  CAT2(a, b)

typedef CAT(PREFIX, div_t) Result;

Result CAT(PREFIX, div)(Integer numerator, Integer denominator)
{
    Result result;

    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    return result;
}
