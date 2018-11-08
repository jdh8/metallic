/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    unsigned char* _getbase;
    unsigned char* _getptr;
    unsigned char* _getend;
    unsigned char* _putbase;
    unsigned char* _putptr;
    unsigned char* _putend;
    int (*_get)(FILE*);
    int (*_put)(int, FILE*);
    size_t (*_read)(void*, size_t, FILE*);
    size_t (*_write)(const void*, size_t, FILE*);
};
