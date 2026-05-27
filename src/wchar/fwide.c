#include "../stdio/FILE.h"
#include <wchar.h>

int fwide(FILE* stream, int mode)
{
    if (stream->orient == 0 && mode != 0)
        stream->orient = mode > 0 ? 1 : -1;
    return stream->orient;
}
