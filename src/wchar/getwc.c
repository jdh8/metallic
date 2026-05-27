#include <wchar.h>
#include <stdio.h>

wint_t getwc(FILE* stream) { return fgetwc(stream); }
