#include <wctype.h>
#include <ctype.h>

int iswgraph(wint_t c) { return c < 128 ? isgraph((int)c) : 0; }
