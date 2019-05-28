#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    unsigned state;

    ptrdiff_t (*const read)(void*, size_t, FILE*);
    ptrdiff_t (*const write)(const void*, size_t, FILE*);
};

enum
{
    _eofbit = 0x10,
    _errbit = 0x20,
};

#endif
