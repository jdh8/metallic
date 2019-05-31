#include "invalid.h"
#include "FILE.h"

size_t __stdio_write(const void*, size_t, FILE*);

FILE __stdout = (FILE){
    .read = __invalid_read,
    .write = __stdio_write,
    .fd = 1
};
