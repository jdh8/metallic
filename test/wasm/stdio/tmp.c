#include <stdio.h>
#include <errno.h>
#include <assert.h>

int main(void)
{
    errno = 0;
    assert(tmpfile() == NULL);
    assert(errno == ENOSYS);

    errno = 0;
    assert(tmpnam(NULL) == NULL);
    assert(errno == ENOSYS);

    return 0;
}
