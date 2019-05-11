#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    int (*const _get)(FILE*);
    int (*const _put)(int, FILE*);
    int (*const _unget)(int, FILE*);
    size_t (*const _read)(void*, size_t, FILE*);
    size_t (*const _write)(const void*, size_t, FILE*);
};

#endif
