#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    int (*_get)(FILE*);
    int (*_put)(int, FILE*);
    int (*_unget)(int, FILE*);
    size_t (*_read)(void*, size_t, FILE*);
    size_t (*_write)(const void*, size_t, FILE*);
};

#endif
