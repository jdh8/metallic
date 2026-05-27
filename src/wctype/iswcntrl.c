#include <wctype.h>
#include <ctype.h>

int iswcntrl(wint_t c) { return c < 128 ? iscntrl((int)c) : 0; }
