#include "invalid.h"
#include "FILE.h"

size_t __stdio_write(const void*, size_t, FILE*);

static FILE _stdout = (FILE){
    .read = __invalid_read,
    .write = __stdio_write,
    .fd = 1
};

FILE* stdout = &_stdout;
