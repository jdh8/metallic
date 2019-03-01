#ifndef _FENV_H
#define _FENV_H

typedef struct { char _; } fenv_t;
typedef unsigned fexcept_t;

#ifdef __cplusplus
extern "C" {
#endif

int feclearexcept(int);
int fetestexcept(int);
int feraiseexcept(int);

int fegetexceptflag(fexcept_t*, int);
int fesetexceptflag(const fexcept_t*, int);

int fsetround(int);
int fgetround(void);

int fegetenv(fenv_t*);
int fesetenv(const fenv_t*);

int feholdexcept(fenv_t*);
int feupdateenv(const fenv_t*);

#ifdef __cplusplus
}
#endif

#define FE_ALL_EXCEPT 0
#define FE_TONEAREST 0
#define FE_DFL_ENV ((const fenv_t*)0)

#endif /* fenv.h */
