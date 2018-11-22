/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_FILE_H
#define METALLIC_FILE_H

#include <stddef.h>

typedef struct __FILE FILE;

struct __FILE
{
    int (*_get)(FILE*);
    int (*_put)(int, FILE*);
    int (*_unget)(int, FILE*);
    size_t (*_read)(void*, size_t, FILE*);
    size_t (*_write)(const void*, size_t, FILE*);
};

#endif
