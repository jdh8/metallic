typedef double time_t;
typedef double clock_t;

#define CLOCKS_PER_SEC 1000.0

extern double _Date_now(void);
extern double clock(void);

inline time_t time(time_t* timer)
{
    time_t result = _Date_now() / 1000;
    if (timer) *timer = result;
    return result;
}
