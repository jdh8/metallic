#include <string.h>

char* strcat(char destination[static restrict 1], const char source[static restrict 1])
{
    strcpy(destination + strlen(destination), source);

    return destination;
}
