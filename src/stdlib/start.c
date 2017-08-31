/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <stdlib.h>

extern void* __stack_pointer;
static void* __heap_pointer;

void* sbrk(ptrdiff_t increment)
{
    const size_t pagesize = 65536;
    size_t end = pagesize * __builtin_wasm_current_memory();
    size_t brk = (size_t)__heap_pointer + increment;

    if (brk > end) {
        size_t excess = brk - end;

        if (__builtin_wasm_grow_memory(excess / pagesize + !!(excess % pagesize)) == -1)
            return (void*) -1;
    }

    return __heap_pointer = (void*) brk;
}

extern int main();

_Noreturn void _start(void)
{
    __heap_pointer = __stack_pointer;
    exit(main());
}
