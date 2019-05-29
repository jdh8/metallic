#include <stdio.h>

int __invalid_get(FILE* stream)
{
    return EOF;
}

int __invalid_put(int c, FILE* stream)
{
    return EOF;
}

size_t __invalid_read(void* buffer, size_t size, FILE* stream)
{
    return 0;
}

size_t __invalid_write(const void* buffer, size_t size, FILE* stream)
{
    return 0;
}
