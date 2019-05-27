#include "putc.h"

int putc(int, FILE[static 1]) __attribute__((alias("_putc")));
int fputc(int, FILE[static 1]) __attribute__((alias("_putc")));
