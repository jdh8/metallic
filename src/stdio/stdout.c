#include "invalid.h"
#include "FILE.h"

int __putchar(int, FILE*);
size_t __stdout(const void*, size_t, FILE*);

static FILE _stdout = (FILE){
    ._get = __invalid_get,
    ._put = __putchar,
    ._unget = __invalid_put,
    ._read = __invalid_read,
    ._write = __stdout
};

FILE* stdout = &_stdout;
