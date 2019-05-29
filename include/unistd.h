#ifndef _UNISTD_H
#define _UNISTD_H

#include "bits/null.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ssize_t;

ssize_t read(int, void*, size_t);
ssize_t write(int, const void*, size_t);

#endif /* unistd.h */

