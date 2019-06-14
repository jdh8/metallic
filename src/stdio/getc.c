#include "getc.h"

int getc(FILE stream[static 1])
{
    return getc_(stream);
}

int fgetc(FILE[static 1]) __attribute__((alias("getc")));
