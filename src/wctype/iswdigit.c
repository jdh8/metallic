#include <wctype.h>
#include <ctype.h>

int iswdigit(wint_t c) { return c < 128 ? isdigit((int)c) : 0; }
