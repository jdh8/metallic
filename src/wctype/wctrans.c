#include <wctype.h>
#include <string.h>

wctrans_t wctrans(const char* name)
{
    if (!strcmp(name, "tolower")) return 1;
    if (!strcmp(name, "toupper")) return 2;
    return 0;
}
