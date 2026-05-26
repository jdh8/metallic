#include <time.h>

/* WASI preview1 has no timezone database, so "local" time is just
 * UTC. Alias gmtime() — but go through a separate static buffer so
 * a program that interleaves gmtime() and localtime() calls (e.g.,
 * `printf("%s -> %s", asctime(gmtime(&t)), asctime(localtime(&t)))`)
 * does not see one trample the other.
 */
struct tm* gmtime(const time_t*);

struct tm* localtime(const time_t* t)
{
    static struct tm result;
    result = *gmtime(t);
    return &result;
}
