#include "putc.h"
#include <stdio.h>

int putchar(int c)
{
    return putc_(c, stdout);
}
