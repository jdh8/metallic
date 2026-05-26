#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Run standalone:
 *     wasmtime test/wasm/stdlib/getenv.out
 * exercises only the "absent variable returns NULL" path, which is what
 * the Makefile's `%.run` rule sees.  Run with an extra `--env FOO=bar`
 * to additionally exercise the "lookup hit" path. */

int main(void)
{
    assert(getenv("NOPE_NONEXISTENT") == NULL);

    const char* foo = getenv("FOO");
    if (foo)
        assert(strcmp(foo, "bar") == 0);

    return 0;
}
