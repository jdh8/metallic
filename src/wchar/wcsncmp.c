#include <wchar.h>

int wcsncmp(const wchar_t lhs[static 1], const wchar_t rhs[static 1], size_t length)
{
    while (length--) {
        wchar_t a = *lhs++;
        wchar_t b = *rhs++;

        if (a != b)
            return a < b ? -1 : 1;

        if (!a)
            return 0;
    }

    return 0;
}
