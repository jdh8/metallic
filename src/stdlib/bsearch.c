#include <stddef.h>

void* bsearch(const void* key, const void* begin, size_t count, size_t size, int compare(const void*, const void*))
{
    if (!count)
        return (void*)0;

    size_t half = count >> 1;
    const void* pivot = (const char*)begin + size * half;
    int comparison = compare(key, pivot);

    if (comparison < 0)
        return bsearch(key, begin, half, size, compare);

    if (comparison)
        return bsearch(key, pivot, count - half, size, compare);

    return (void*)pivot;
}
