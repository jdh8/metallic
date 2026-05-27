#include <stdlib.h>
#include <limits.h>
#include <assert.h>

int main(void)
{
    assert(MB_CUR_MAX == MB_LEN_MAX);
    assert(MB_CUR_MAX == 4);
    return 0;
}
