#include <stdlib.h>

/* `getenv` is a standard libc function, so its body lives in metallic.a.
 * The actual environ table is populated by the WASI backend (src/wasi/
 * environ.c), which exposes `__environ_find` as the host-agnostic lookup
 * entry point.  This keeps the WASI dependency one-way: stdlib references
 * the backend symbol, never the other way round. */

extern char* __environ_find(const char* name);

char* getenv(const char* name)
{
    return name ? __environ_find(name) : (char*)0;
}
