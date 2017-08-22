/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void* lhs, const void* rhs, size_t length);
void* memcpy(void*__restrict destination, const void*__restrict source, size_t length);
void* memmove(void* destination, const void* source, size_t length);
void* memset(void* destination, int c, size_t length);

#ifdef _GNU_SOURCE
void* __mempcpy(void*__restrict destination, const void*__restrict source, size_t length);

inline void* mempcpy(void*__restrict destination, const void*__restrict source, size_t length)
{ return __mempcpy(destination, source, length); }
#endif

#ifdef __cplusplus
namespace __metallic {
#endif

void* memchr(const void* source, int c, size_t length);

#ifdef __cplusplus
} // namespace __metallic

extern "C++" inline
const void* memchr(const void* source, int c, size_t length)
{ return __metallic::memchr(source, c, length); }

extern "C++" inline
void* memchr(void* source, int c, size_t length)
{ return __metallic::memchr(source, c, length); }

} // extern "C"
#endif
