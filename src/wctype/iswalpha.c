#include <wctype.h>
#include <ctype.h>

int iswalpha(wint_t c) { return c < 128 ? isalpha((int)c) : 0; }
