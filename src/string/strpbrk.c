#include <string.h>

char* strpbrk(const char string[static 1], const char set[static 1])
{
    while (!strchr(set, *string))
        ++string;

    return *string ? (char*)string : 0;
}
