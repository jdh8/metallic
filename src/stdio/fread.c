#include "FILE.h"
#include <string.h>

static size_t wrapper_(unsigned char* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    size_t hit = stream->avail < size ? stream->avail : size;
    size_t missed = size - hit;

    memcpy(buffer, stream->cache + sizeof(stream->cache) - stream->avail, hit);
    stream->avail -= hit;
    return hit + (missed ? stream->read(buffer + hit, missed, stream) : 0);
}

size_t fread(void* restrict buffer, size_t size, size_t count, FILE stream[restrict static 1])
{
    return wrapper_(buffer, size * count, stream) / size;
}
