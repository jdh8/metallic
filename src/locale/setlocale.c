#include <locale.h>
#include <string.h>

/* C11 §7.11.1.1. The only locale we implement is the C locale.
 * setlocale(category, NULL) queries — return "C".
 * setlocale(category, "") or setlocale(category, "C") sets — return "C".
 * Anything else is unsupported and we return NULL (per the spec: "If
 * the selection cannot be honored, the setlocale function returns a
 * null pointer"). */
char* setlocale(int category, const char* locale)
{
    if (category < LC_ALL || category > LC_TIME)
        return NULL;

    if (locale == NULL)
        return (char*)"C";

    if (locale[0] == '\0' || strcmp(locale, "C") == 0
                          || strcmp(locale, "POSIX") == 0)
        return (char*)"C";

    return NULL;
}
