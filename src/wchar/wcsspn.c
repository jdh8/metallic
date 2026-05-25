#include <wchar.h>

size_t wcsspn(const wchar_t string[static 1], const wchar_t accept[static 1])
{
    size_t count = 0;

    while (string[count] && wcschr(accept, string[count]))
        ++count;

    return count;
}
