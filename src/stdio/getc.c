#include "getc.h"

int getc(FILE[static 1]) __attribute__((alias("_getc")));
int fgetc(FILE[static 1]) __attribute__((alias("_getc")));
