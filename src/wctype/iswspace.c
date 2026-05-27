#include <wctype.h>
#include <ctype.h>

int iswspace(wint_t c) { return c < 128 ? isspace((int)c) : 0; }
