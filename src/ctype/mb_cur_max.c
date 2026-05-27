#include <limits.h>
#include <stddef.h>

/* Backing function for the MB_CUR_MAX macro.  Metallic always speaks UTF-8
 * (even in the C locale, which is the only locale supported), so the answer
 * is constant. */
size_t __ctype_get_mb_cur_max(void)
{
    return MB_LEN_MAX;
}
