#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    unsigned state;

    int (*const get)(FILE*);
    int (*const put)(int, FILE*);
    int (*const unget)(int, FILE*);
    size_t (*const read)(void*, size_t, FILE*);
    size_t (*const write)(const void*, size_t, FILE*);
};

enum
{
    _eofbit = 0x10,
    _errbit = 0x20,
};

#endif
