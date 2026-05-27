#include <wchar.h>
#include <stdio.h>

wint_t putwc(wchar_t c, FILE* stream) { return fputwc(c, stream); }
