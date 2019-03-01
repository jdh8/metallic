#include <string.h>

char* strrchr(const char source[static 1], int character)
{
    unsigned char c = character;

    if (!c)
        return (char*)(source + strlen(source));

    char* found = 0;
    char* candidate;

    while ((candidate = strchr(source, character))) {
        found = candidate;
        source = candidate + 1;
    }

    return found;
}
