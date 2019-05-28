#include "invalid.h"
#include "FILE.h"

ptrdiff_t __stdout(const void*, size_t, FILE*);

static FILE _stdout = (FILE){
    .read = __invalid_read,
    .write = __stdout
};

FILE* stdout = &_stdout;
