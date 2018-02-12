/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STRING_H
#define _STRING_H

#include "bits/specifiers.h"

typedef __SIZE_TYPE__ size_t;

int memcmp(const void*, const void*, size_t);
void* memchr(const void*, int, size_t);
void* memset(void*, int, size_t);
void* memcpy(void*__restrict, const void*__restrict, size_t);
void* memmove(void*, const void*, size_t);

size_t strlen(const char[_STATIC 1]);

int strcmp(const char[_STATIC 1], const char rhs[_STATIC 1]);
int strncmp(const char[_STATIC 1], const char rhs[_STATIC 1], size_t);

char* strchr(const char[_STATIC 1], int);
char* strrchr(const char[_STATIC 1], int);

char* strcpy(char[_STATIC _RESTRICT 1], const char[_STATIC _RESTRICT 1]);
char* strncpy(char[_STATIC _RESTRICT 1], const char[_STATIC _RESTRICT 1], size_t);

char* strcat(char[_STATIC _RESTRICT 1], const char[_STATIC _RESTRICT 1]);
char* strncat(char[_STATIC _RESTRICT 1], const char[_STATIC _RESTRICT 1], size_t);

size_t strspn(const char[_STATIC 1], const char[_STATIC 1]);
size_t strcspn(const char[_STATIC 1], const char[_STATIC 1]);
char* strpbrk(const char[_STATIC 1], const char[_STATIC 1]);

char* strstr(const char[_STATIC 1], const char[_STATIC 1]);
char* strtok(char*__restrict, const char*__restrict);

#endif /* string.h */
