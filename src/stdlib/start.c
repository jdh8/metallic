/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>
#include <stdlib.h>

void* sbrk(intptr_t increment)
{
    const size_t pagesize = 64 * 1024;
    static uintptr_t heap = 0;
    uintptr_t top = pagesize * __builtin_wasm_current_memory();

    if (heap + increment > top) {
        uintptr_t overflow = heap + increment - top;
        size_t pages = overflow / pagesize + !!(overflow % pagesize);

        if (__builtin_wasm_grow_memory(pages) == -1)
            return (void*) -1;
    }

    return (void*)(heap += increment);
}

extern int main(void);

_Noreturn void _start(void)
{
    sbrk((intptr_t)__builtin_frame_address(0));
    exit(main());
}
