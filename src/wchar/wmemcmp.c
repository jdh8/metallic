#include <wchar.h>

int wmemcmp(const wchar_t* lhs, const wchar_t* rhs, size_t length)
{
    while (length--) {
        wchar_t a = *lhs++;
        wchar_t b = *rhs++;

        if (a != b)
            return a < b ? -1 : 1;
    }

    return 0;
}
