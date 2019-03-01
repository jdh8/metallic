#include <fenv.h>

int fesetexceptflag(const fexcept_t* flags, int excepts)
{
    return *flags & excepts;
}
