#include <time.h>

extern double _Date_now(void);

time_t time(time_t* timer)
{
    time_t result = _Date_now() / 1000;
    if (timer) *timer = result;
    return result;
}
