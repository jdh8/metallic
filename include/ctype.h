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

int iscntrl(int c);
int isprint(int c);
int isblank(int c);
int isspace(int c);
int isgraph(int c);

int isupper(int c);
int islower(int c);
int isalpha(int c);

int isdigit(int c);
int isxdigit(int c);

int isalnum(int c);
int ispunct(int c);

#ifdef __cplusplus
}
#endif

#endif /* ctype.h */
