/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STRING_H
#define _STRING_H

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void*__restrict, const void*__restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);

#ifdef _GNU_SOURCE
void* __mempcpy(void*__restrict, const void*__restrict, size_t);

inline void* mempcpy(void*__restrict destination, const void*__restrict source, size_t length)
{ return __mempcpy(destination, source, length); }
#endif

#ifdef __cplusplus
namespace __metallic {
#endif

void* memchr(const void*, int, size_t);

#ifdef __cplusplus
} // namespace __metallic
} // extern "C"

inline const void* memchr(const void* source, int c, size_t length) { return __metallic::memchr(source, c, length); }
inline void* memchr(void* source, int c, size_t length) { return __metallic::memchr(source, c, length); }
#endif

#endif /* string.h */
