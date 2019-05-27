#include "FILE.h"

size_t fread(void* restrict buffer, size_t size, size_t count, FILE stream[restrict static 1])
{
    return stream->read(buffer, size * count, stream) / size;
}
