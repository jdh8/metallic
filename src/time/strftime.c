#include <time.h>
#include <string.h>

/* Minimal C-locale strftime. Supports the subset of conversions
 * required for C11's hosted environment and a handful of useful
 * POSIX extensions: %Y %y %m %d %H %I %M %S %j %p %A %a %B %b
 * %c %x %X %% %F %T %R %e %u %w %Z %n %t. Unsupported specifiers
 * are passed through verbatim, matching glibc's permissive behaviour.
 */

static const char* const wday_full[] = {
    "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
};
static const char* const wday_abbr[] = {
    "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
};
static const char* const mon_full[] = {
    "January","February","March","April","May","June",
    "July","August","September","October","November","December"
};
static const char* const mon_abbr[] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

/* Append a single character. Returns 0 on success, 1 if the output
 * has overflowed (size_t-saturating bookkeeping handled by caller). */
static int append_char(char* buf, size_t max, size_t* pos, char c)
{
    if (*pos >= max) return 1;
    buf[*pos] = c;
    *pos += 1;
    return 0;
}

static int append_str(char* buf, size_t max, size_t* pos, const char* s)
{
    while (*s) {
        if (append_char(buf, max, pos, *s++)) return 1;
    }
    return 0;
}

/* Format an unsigned integer right-justified in `width` columns, padded
 * with `pad` (' ' or '0'). width <= 0 means no padding. */
static int append_uint(char* buf, size_t max, size_t* pos,
                       unsigned long n, int width, char pad)
{
    char tmp[20];
    int len = 0;
    if (n == 0) {
        tmp[len++] = '0';
    } else {
        while (n > 0) {
            tmp[len++] = (char)('0' + n % 10);
            n /= 10;
        }
    }
    while (len < width) {
        tmp[len++] = pad;
    }
    while (len > 0) {
        if (append_char(buf, max, pos, tmp[--len])) return 1;
    }
    return 0;
}

/* Forward declaration so format() can recurse for composite specifiers
 * like %c, %F, %T. */
static int format(char* buf, size_t max, size_t* pos,
                  const char* fmt, const struct tm* tm);

static int emit_one(char* buf, size_t max, size_t* pos,
                    char spec, const struct tm* tm)
{
    switch (spec) {
    case 'Y':
        return append_uint(buf, max, pos, (unsigned long)(tm->tm_year + 1900), 4, '0');
    case 'y':
        return append_uint(buf, max, pos, (unsigned long)((tm->tm_year + 1900) % 100), 2, '0');
    case 'C':
        return append_uint(buf, max, pos, (unsigned long)((tm->tm_year + 1900) / 100), 2, '0');
    case 'm':
        return append_uint(buf, max, pos, (unsigned long)(tm->tm_mon + 1), 2, '0');
    case 'd':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_mday, 2, '0');
    case 'e':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_mday, 2, ' ');
    case 'H':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_hour, 2, '0');
    case 'I': {
        int h = tm->tm_hour % 12;
        if (h == 0) h = 12;
        return append_uint(buf, max, pos, (unsigned long)h, 2, '0');
    }
    case 'M':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_min, 2, '0');
    case 'S':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_sec, 2, '0');
    case 'j':
        return append_uint(buf, max, pos, (unsigned long)(tm->tm_yday + 1), 3, '0');
    case 'p':
        return append_str(buf, max, pos, tm->tm_hour < 12 ? "AM" : "PM");
    case 'A':
        if (tm->tm_wday < 0 || tm->tm_wday > 6) return 0;
        return append_str(buf, max, pos, wday_full[tm->tm_wday]);
    case 'a':
        if (tm->tm_wday < 0 || tm->tm_wday > 6) return 0;
        return append_str(buf, max, pos, wday_abbr[tm->tm_wday]);
    case 'B':
        if (tm->tm_mon < 0 || tm->tm_mon > 11) return 0;
        return append_str(buf, max, pos, mon_full[tm->tm_mon]);
    case 'b':
    case 'h':
        if (tm->tm_mon < 0 || tm->tm_mon > 11) return 0;
        return append_str(buf, max, pos, mon_abbr[tm->tm_mon]);
    case 'c':
        return format(buf, max, pos, "%a %b %e %H:%M:%S %Y", tm);
    case 'x':
        return format(buf, max, pos, "%m/%d/%y", tm);
    case 'X':
        return format(buf, max, pos, "%H:%M:%S", tm);
    case 'F':
        return format(buf, max, pos, "%Y-%m-%d", tm);
    case 'T':
        return format(buf, max, pos, "%H:%M:%S", tm);
    case 'R':
        return format(buf, max, pos, "%H:%M", tm);
    case 'D':
        return format(buf, max, pos, "%m/%d/%y", tm);
    case 'r':
        return format(buf, max, pos, "%I:%M:%S %p", tm);
    case 'u': {
        /* ISO 8601: 1=Monday..7=Sunday. */
        int u = tm->tm_wday == 0 ? 7 : tm->tm_wday;
        return append_uint(buf, max, pos, (unsigned long)u, 0, ' ');
    }
    case 'w':
        return append_uint(buf, max, pos, (unsigned long)tm->tm_wday, 0, ' ');
    case 'Z':
        return 0; /* no timezone info under WASI preview1 */
    case 'n':
        return append_char(buf, max, pos, '\n');
    case 't':
        return append_char(buf, max, pos, '\t');
    case '%':
        return append_char(buf, max, pos, '%');
    default:
        /* Echo unknown specifier verbatim so the caller can spot it. */
        if (append_char(buf, max, pos, '%')) return 1;
        return append_char(buf, max, pos, spec);
    }
}

static int format(char* buf, size_t max, size_t* pos,
                  const char* fmt, const struct tm* tm)
{
    while (*fmt) {
        if (*fmt != '%') {
            if (append_char(buf, max, pos, *fmt++)) return 1;
            continue;
        }
        ++fmt;
        /* Skip an optional 'E' or 'O' modifier; C11 says the
         * implementation may treat them as if the modifier were absent. */
        if (*fmt == 'E' || *fmt == 'O') ++fmt;
        if (*fmt == 0) {
            /* Trailing '%' with nothing after — emit it literally. */
            return append_char(buf, max, pos, '%');
        }
        if (emit_one(buf, max, pos, *fmt, tm)) return 1;
        ++fmt;
    }
    return 0;
}

size_t strftime(char* __restrict s, size_t maxsize,
                const char* __restrict fmt, const struct tm* __restrict tm)
{
    if (maxsize == 0) return 0;
    size_t pos = 0;
    /* Reserve one byte for the terminating NUL by formatting into
     * maxsize-1 slots. */
    int overflow = format(s, maxsize - 1, &pos, fmt, tm);
    if (overflow) return 0;
    s[pos] = '\0';
    return pos;
}
