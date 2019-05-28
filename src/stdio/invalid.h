#include <stddef.h>

typedef struct __FILE FILE;

int __invalid_get(FILE*);
int __invalid_put(int, FILE*);
ptrdiff_t __invalid_read(void*, size_t, FILE*);
ptrdiff_t __invalid_write(const void*, size_t, FILE*);
