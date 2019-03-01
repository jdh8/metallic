#include <stddef.h>

int memcmp(const void* lhs, const void* rhs, size_t length)
{
    const unsigned char* x = lhs;
    const unsigned char* y = rhs;

    while (length--) {
        unsigned char a = *x++;
        unsigned char b = *y++;

        if (a != b)
            return a - b;
    }

    return 0;
}

