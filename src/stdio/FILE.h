#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>
#include <limits.h>

typedef long long off_t;
typedef struct __FILE FILE;

struct __FILE
{
    unsigned state;
    unsigned avail;
    unsigned char cache[4 * MB_LEN_MAX];

    unsigned char* base;
    unsigned char* ptr;
    unsigned char* end;

    size_t (*read)(FILE*, void*, size_t);
    size_t (*write)(FILE*, const void*, size_t);
    off_t (*seek)(FILE*, off_t, int);
    int (*close)(FILE*);

    int fd;
};

enum
{
    eofbit_ = 0x10,
    errbit_ = 0x20,
    appbit_ = 0x80,
};

#endif
