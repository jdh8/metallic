#include <wchar.h>

int wcscoll(const wchar_t lhs[static 1], const wchar_t rhs[static 1])
{
    /* C locale: collation == codepoint comparison */
    return wcscmp(lhs, rhs);
}
