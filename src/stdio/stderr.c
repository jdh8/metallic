#include "invalid.h"
#include "FILE.h"

int __putcerr(int, FILE*);
size_t __stderr(const void*, size_t, FILE*);

static FILE _stderr = (FILE){
    ._get = __invalid_get,
    ._put = __putcerr,
    ._unget = __invalid_put,
    ._read = __invalid_read,
    ._write = __stderr
};

FILE* stderr = &_stderr;
