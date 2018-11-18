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

int __invalid_get(FILE*);
int __invalid_put(int, FILE*);
size_t __invalid_read(void*, size_t, FILE*);
size_t __invalid_write(const void*, size_t, FILE*);
