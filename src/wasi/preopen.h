#ifndef METALLIC_WASI_PREOPEN_H
#define METALLIC_WASI_PREOPEN_H

#include <stddef.h>

struct preopen {
    int fd;
    const char *prefix;
    size_t prefix_len;
};

/* Look up the preopen that contains `path`. On success: returns 0,
 * *out_basefd is the preopen fd, *out_rel and *out_rel_len point into
 * `path` for the path relative to the preopen. On miss: returns
 * negative POSIX errno (-ENOENT). */
int preopen_lookup(const char *path, int *out_basefd,
                   const char **out_rel, size_t *out_rel_len);

#endif /* METALLIC_WASI_PREOPEN_H */
