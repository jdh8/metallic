#include <wctype.h>

wint_t towctrans(wint_t c, wctrans_t trans)
{
    switch (trans) {
        case 1: return towlower(c);
        case 2: return towupper(c);
    }
    return c;
}
