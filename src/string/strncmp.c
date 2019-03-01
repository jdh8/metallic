#include <stddef.h>

int strncmp(const char lhs[static 1], const char rhs[static 1], size_t length)
{
    const unsigned char* x = (const unsigned char*)lhs;
    const unsigned char* y = (const unsigned char*)rhs;

    while (length--) {
        unsigned char a = *x++;
        unsigned char b = *y++;

        if (a != b || !a)
            return a - b;
    }

    return 0;
}

