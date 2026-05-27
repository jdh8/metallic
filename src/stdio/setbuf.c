#include <stdio.h>

void setbuf(FILE* restrict stream, char* restrict buf)
{
    setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}
