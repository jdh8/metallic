#include "FILE.h"

size_t fwrite(const void* restrict buffer, size_t size, size_t count, FILE stream[restrict static 1])
{
    return stream->_write(buffer, size * count, stream) / size;
}
