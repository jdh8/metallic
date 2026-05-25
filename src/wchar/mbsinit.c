#include <wchar.h>

int mbsinit(const mbstate_t* ps)
{
    return !ps || ps->__code == 0;
}
