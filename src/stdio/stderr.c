#include "invalid.h"
#include "FILE.h"

ptrdiff_t __stderr(const void*, size_t, FILE*);

static FILE _stderr = (FILE){
    .read = __invalid_read,
    .write = __stderr
};

FILE* stderr = &_stderr;
