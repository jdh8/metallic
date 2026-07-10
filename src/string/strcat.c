#include <string.h>

char* strcat(char destination[static restrict 1], const char source[static restrict 1])
{
    memcpy(destination + strlen(destination), source, strlen(source) + 1);

    return destination;
}
