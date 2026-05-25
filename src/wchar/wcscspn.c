#include <wchar.h>

size_t wcscspn(const wchar_t string[static 1], const wchar_t reject[static 1])
{
    size_t count = 0;

    while (string[count] && !wcschr(reject, string[count]))
        ++count;

    return count;
}
