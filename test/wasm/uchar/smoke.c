#include <uchar.h>

_Static_assert(sizeof(char16_t) == 2, "char16_t must be 16-bit");
_Static_assert(sizeof(char32_t) == 4, "char32_t must be 32-bit");

int main(void) { return 0; }
