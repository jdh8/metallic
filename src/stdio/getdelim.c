#include "getc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

ssize_t getdelim(char** restrict line, size_t* restrict cap, int delim, FILE* restrict stream)
{
    if (!line || !cap) {
        errno = EINVAL;
        return -1;
    }

    if (!*line || !*cap) {
        size_t init = 128;
        char* p = realloc(*line, init);
        if (!p)
            return -1;
        *line = p;
        *cap = init;
    }

    size_t n = 0;
    for (;;) {
        int c = getc_(stream);
        if (c == EOF) {
            if (!n)
                return -1;
            break;
        }

        if (n + 1 >= *cap) {
            size_t need = *cap * 2;
            char* p = realloc(*line, need);
            if (!p)
                return -1;
            *line = p;
            *cap = need;
        }

        (*line)[n++] = (char)c;
        if (c == delim)
            break;
    }

    (*line)[n] = '\0';
    return (ssize_t)n;
}
