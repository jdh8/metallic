#include <wchar.h>

int wcscmp(const wchar_t lhs[static 1], const wchar_t rhs[static 1])
{
    while (*lhs == *rhs && *lhs) {
        ++lhs;
        ++rhs;
    }

    if (*lhs == *rhs)
        return 0;

    return *lhs < *rhs ? -1 : 1;
}
