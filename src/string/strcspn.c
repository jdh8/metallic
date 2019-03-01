#include <string.h>

size_t strcspn(const char string[static 1], const char reject[static 1])
{
    size_t count = 0;

    while (!strchr(reject, string[count]))
        ++count;

    return count;
}
