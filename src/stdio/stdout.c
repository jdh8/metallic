#include "invalid.h"
#include "FILE.h"

int __putchar(int, FILE*);
size_t __stdout(const void*, size_t, FILE*);

static FILE _stdout = (FILE){
    .get = __invalid_get,
    .put = __putchar,
    .unget = __invalid_put,
    .read = __invalid_read,
    .write = __stdout
};

FILE* stdout = &_stdout;
