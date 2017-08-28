/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <ctype.h>

extern inline int iscntrl(int c);
extern inline int isprint(int c);
extern inline int isblank(int c);
extern inline int isspace(int c);
extern inline int isgraph(int c);

extern inline int isupper(int c);
extern inline int islower(int c);
extern inline int isalpha(int c);

extern inline int isdigit(int c);
extern inline int isxdigit(int c);

extern inline int isalnum(int c);
extern inline int ispunct(int c);
