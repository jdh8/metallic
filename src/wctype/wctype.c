#include <wctype.h>
#include <string.h>

/* C11 §7.30.2.2.1 requires recognition of these property names; the
 * returned token is opaque, so the numbering is arbitrary as long as
 * iswctype agrees. */
wctype_t wctype(const char* name)
{
    if (!strcmp(name, "alnum"))  return 1;
    if (!strcmp(name, "alpha"))  return 2;
    if (!strcmp(name, "blank"))  return 3;
    if (!strcmp(name, "cntrl"))  return 4;
    if (!strcmp(name, "digit"))  return 5;
    if (!strcmp(name, "graph"))  return 6;
    if (!strcmp(name, "lower"))  return 7;
    if (!strcmp(name, "print"))  return 8;
    if (!strcmp(name, "punct"))  return 9;
    if (!strcmp(name, "space"))  return 10;
    if (!strcmp(name, "upper"))  return 11;
    if (!strcmp(name, "xdigit")) return 12;
    return 0;
}
