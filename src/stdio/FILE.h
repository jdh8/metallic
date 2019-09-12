#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    unsigned state;

    size_t (*read)(void*, size_t, FILE*);
    size_t (*write)(const void*, size_t, FILE*);
    long (*seek)(FILE*, long, int);

    int fd;
};

enum
{
    eofbit_ = 0x10,
    errbit_ = 0x20,
};

#endif
