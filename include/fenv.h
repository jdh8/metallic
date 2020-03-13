#ifndef _FENV_H
#define _FENV_H

typedef struct { int: 0; } fenv_t;
typedef unsigned fexcept_t;

#define FE_ALL_EXCEPT 0
#define FE_TONEAREST 0
#define FE_DFL_ENV ((const fenv_t*)0)

#ifdef __cplusplus
extern "C" {
#endif

inline int feclearexcept(int _excepts)
{
    return _excepts;
}

inline int fetestexcept(int _excepts)
{
    return 0;
}

inline int feraiseexcept(int _excepts)
{
    return _excepts;
}

inline int fegetexceptflag(fexcept_t* _flags, int _excepts)
{
    return 0;
}

inline int fesetexceptflag(const fexcept_t* _flags, int _excepts)
{
    return *_flags & _excepts;
}

inline int fesetround(int _round)
{
    return _round;
}

inline int fegetround(void)
{
    return FE_TONEAREST;
}

inline int fegetenv(fenv_t* _env)
{
    return 0;
}

inline int fesetenv(const fenv_t* _env)
{
    return 0;
}

inline int feholdexcept(fenv_t* _env)
{
    return 0;
}

inline int feupdateenv(const fenv_t* _env)
{
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* fenv.h */
