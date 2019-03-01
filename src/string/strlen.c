#include <stddef.h>
#include <stdint.h>

size_t strlen(const char begin[static 1])
{
    const char* end = begin;

    for (; (uintptr_t)end % sizeof(uint64_t); ++end)
        if (!*end)
            return end - begin;

    const uint64_t magic = 0x7EFEFEFEFEFEFEFF;

    for (const uint64_t* vector = (const uint64_t*)end; ; end = (const char*)++vector) {
        if (((*vector + magic) ^ ~*vector) & ~magic)
            for (int k = 0; k < sizeof(uint64_t); ++k)
                if (!end[k])
                    return end - begin + k;
    }
}
