#include "../FILE.h"

size_t __sstream_read(void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    unsigned char* destination = buffer;
    size_t count = 0;

    for (; count < size && *stream->ptr; ++count)
        *destination++ = *stream->ptr++;

    return count;
}
