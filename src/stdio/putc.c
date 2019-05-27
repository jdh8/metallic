#include "putc.h"

int putc(int c, FILE stream[static 1])
{
    return _putc(c, stream);
}

int fputc(int, FILE[static 1]) __attribute__((alias("putc")));
