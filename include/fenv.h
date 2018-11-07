/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _FENV_H
#define _FENV_H

typedef struct {} fenv_t;
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
