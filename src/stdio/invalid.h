#include <stddef.h>

typedef struct __FILE FILE;

int __invalid_get(FILE*);
int __invalid_put(int, FILE*);
size_t __invalid_read(void*, size_t, FILE*);
size_t __invalid_write(const void*, size_t, FILE*);
