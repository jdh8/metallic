#include <wctype.h>
#include <ctype.h>

int iswalnum(wint_t c) { return c < 128 ? isalnum((int)c) : 0; }
