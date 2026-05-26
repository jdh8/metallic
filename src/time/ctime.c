#include <time.h>

extern struct tm* localtime(const time_t*);
extern char* asctime(const struct tm*);

char* ctime(const time_t* t)
{
    return asctime(localtime(t));
}
