#include <ctype.h>

Integer ATOI(const char s[static 1])
{
    Integer magnitude = 0;
    Integer sign = 1;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    for (unsigned digit = *s - '0'; digit < 10; digit = *s - '0') {
        magnitude = 10 * magnitude + digit;
        ++s;
    }

    return sign * magnitude;
}
