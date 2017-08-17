#include "compare.h"
#include <stddef.h>

int memcmp(const void* lhs, const void* rhs, size_t length)
{
    const unsigned char* a = lhs;
    const unsigned char* b = rhs;

    for (int sign; length; --length)
        if ((sign = compare(*a++, *b++)))
            return sign;

    return 0;
}

