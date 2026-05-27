#include <wchar.h>
#include <stdio.h>

wint_t getwchar(void) { return fgetwc(stdin); }
