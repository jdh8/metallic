/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static const uintptr_t pagesize = 64 * 1024;

void* sbrk(intptr_t increment)
{
    static uintptr_t top = 0;
    uintptr_t previous = top;
    uintptr_t capacity = pagesize * __builtin_wasm_memory_size(0);

    if (top + increment > capacity) {
        uintptr_t excess = top + increment - capacity;
        uintptr_t pages = excess / pagesize + !!(excess % pagesize);

        if (__builtin_wasm_memory_grow(0, pages) == -1)
            return (void*)-1;
    }
    top += increment;
    return (void*)previous;
}

__attribute__((constructor))
static void _initialize(void)
{
    sbrk(pagesize * __builtin_wasm_memory_size(0));
}
