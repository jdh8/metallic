#include "invalid.h"
#include "stdout.h"
#include "FILE.h"
#include <stdio.h>

static int _put(int c, FILE* stream)
{
    return putchar(c);
}

static FILE _stdout = (FILE){
    ._get = __invalid_get,
    ._put = _put,
    ._unget = __invalid_put,
    ._read = __invalid_read,
    ._write = __stdout
};

FILE* stdout = &_stdout;
