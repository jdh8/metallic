#include <stddef.h>

static const char* find_(const char source[static 1], char character)
{
    for (; *source; ++source)
        if (*source == character)
            return source;

    return 0;
}

size_t strspn(const char string[static 1], const char accept[static 1])
{
    size_t count = 0;

    while (find_(accept, string[count]))
        ++count;

    return count;
}
