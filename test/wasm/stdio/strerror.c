#include <string.h>
#include <errno.h>
#include <assert.h>

int main(void)
{
    assert(strerror(0)[0] != '\0');
    assert(strerror(ENOENT)[0] != '\0');
    assert(strerror(EINVAL)[0] != '\0');
    assert(strerror(99999)[0] != '\0');  /* unknown errno still returns a string */

    /* sanity-check a couple of known mappings */
    const char* nf = strerror(ENOENT);
    assert(nf[0] == 'N');

    return 0;
}
