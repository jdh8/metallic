#include <string.h>

char* strtok_r(char* restrict string, const char* restrict separators, char* state[static restrict 1])
{
    if (!(string || (string = *state)))
        return 0;

    string += strspn(string, separators);

    if (!*string)
        return *state = 0;

    *state = strpbrk(string, separators);

    if (*state)
        *(*state)++ = '\0';

    return string;
}

char* strtok(char* restrict string, const char* restrict separators)
{
    static char* state;

    return strtok_r(string, separators, &state);
}
