#include <string.h>
#include "../FILE.h"

size_t __sstream_write(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    memcpy(stream->ptr, buffer, size);
    stream->ptr += size;
    return size;
}
