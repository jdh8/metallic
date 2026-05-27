#include <stdio.h>
#include <assert.h>

int main(void)
{
    char buf[256];

    assert(setvbuf(stdout, buf, _IOFBF, sizeof buf) == 0);
    assert(setvbuf(stdout, NULL, _IOLBF, 0) == 0);
    assert(setvbuf(stdout, NULL, _IONBF, 0) == 0);
    assert(setvbuf(stdout, NULL, 99, 0) != 0);  /* invalid mode */

    setbuf(stdout, NULL);
    setbuf(stdout, buf);

    /* Writes should still work after setvbuf/setbuf. */
    int n = printf("");
    assert(n == 0);

    return 0;
}
