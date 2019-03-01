#include <string.h>

char* strncat(char destination[static restrict 1], const char source[static restrict 1], size_t length)
{
    char* output = destination + strlen(destination);

    while (length-- && (*output++ = *source++));

    return destination;
}
