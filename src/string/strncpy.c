#include <string.h>

char* strncpy(char destination[static restrict 1], const char source[static restrict 1], size_t length)
{
    char* output = destination;

    for (; length && *source; --length)
        *output++ = *source++;

    if (length) // clang is happier with this
        memset(output, 0, length);

    return destination;
}
