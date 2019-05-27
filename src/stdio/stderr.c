#include "invalid.h"
#include "FILE.h"

int __putcerr(int, FILE*);
size_t __stderr(const void*, size_t, FILE*);

static FILE _stderr = (FILE){
    .get = __invalid_get,
    .put = __putcerr,
    .unget = __invalid_put,
    .read = __invalid_read,
    .write = __stderr
};

FILE* stderr = &_stderr;
