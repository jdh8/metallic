#include "fwrite.h"

size_t fwrite(const void* restrict buffer, size_t size, size_t count, FILE stream[restrict static 1])
{
    return _fwrite(buffer, size * count, stream) / size;
}
