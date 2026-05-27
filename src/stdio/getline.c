#include <stdio.h>

ssize_t getline(char** restrict line, size_t* restrict cap, FILE* restrict stream)
{
    return getdelim(line, cap, '\n', stream);
}
