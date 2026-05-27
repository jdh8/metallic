#include <wctype.h>

int iswctype(wint_t c, wctype_t type)
{
    switch (type) {
        case 1:  return iswalnum(c);
        case 2:  return iswalpha(c);
        case 3:  return iswblank(c);
        case 4:  return iswcntrl(c);
        case 5:  return iswdigit(c);
        case 6:  return iswgraph(c);
        case 7:  return iswlower(c);
        case 8:  return iswprint(c);
        case 9:  return iswpunct(c);
        case 10: return iswspace(c);
        case 11: return iswupper(c);
        case 12: return iswxdigit(c);
    }
    return 0;
}
