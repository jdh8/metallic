#include <string.h>

/* C locale only: collation order is byte order, identical to strcmp. */
int strcoll(const char* a, const char* b)
{
    return strcmp(a, b);
}
