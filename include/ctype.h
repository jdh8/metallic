/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _CTYPE_H
#define _CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

int iscntrl(int);
int isprint(int);
int isblank(int);
int isspace(int);
int isgraph(int);

int isupper(int);
int islower(int);
int isalpha(int);

int isdigit(int);
int isxdigit(int);

int isalnum(int);
int ispunct(int);

int toupper(int);
int tolower(int);

#ifdef __cplusplus
}
#endif

#endif /* ctype.h */
