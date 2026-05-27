#include <wctype.h>
#include <ctype.h>

int iswprint(wint_t c) { return c < 128 ? isprint((int)c) : 0; }
