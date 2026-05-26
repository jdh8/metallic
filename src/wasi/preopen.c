#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "wasi.h"
#include "preopen.h"

#define MAX_PREOPENS 16
#define POOL_SIZE    1024

static struct preopen entries[MAX_PREOPENS];
static int count = -1;
static char pool[POOL_SIZE];
static size_t pool_used = 0;

static const char *pool_dup(const uint8_t *buf, size_t len)
{
    /* Reserve one extra byte for NUL so prefix strings remain printable
     * if anyone treats them as C strings. */
    if (pool_used + len + 1 > POOL_SIZE)
        return NULL;
    char *out = pool + pool_used;
    for (size_t i = 0; i < len; i++)
        out[i] = (char)buf[i];
    out[len] = '\0';
    pool_used += len + 1;
    return out;
}

static void ensure_init(void)
{
    if (count >= 0)
        return;

    int n = 0;
    for (__wasi_fd_t fd = 3; n < MAX_PREOPENS; fd++) {
        __wasi_prestat_t pst;
        __wasi_errno_t e = __wasi_fd_prestat_get(fd, &pst);
        if (e == __WASI_ERRNO_BADF)
            break;
        if (e != 0)
            continue;
        if (pst.tag != 0)
            continue;

        size_t plen = pst.u.dir.pr_name_len;
        uint8_t buf[256];
        if (plen > sizeof(buf))
            plen = sizeof(buf);
        e = __wasi_fd_prestat_dir_name(fd, buf, plen);
        if (e != 0)
            continue;

        /* Trim any trailing '/' for consistent comparison. */
        while (plen > 0 && buf[plen - 1] == '/')
            plen--;

        const char *p = pool_dup(buf, plen);
        if (!p)
            break;

        entries[n].fd = (int)fd;
        entries[n].prefix = p;
        entries[n].prefix_len = plen;
        n++;
    }

    count = n;
}

static const char *strip_dotslash(const char *path)
{
    while (path[0] == '.' && path[1] == '/') {
        path += 2;
        while (path[0] == '/')
            path++;
    }
    return path;
}

int preopen_lookup(const char *path, int *out_basefd,
                   const char **out_rel, size_t *out_rel_len)
{
    ensure_init();

    if (!path || !*path) {
        errno = ENOENT;
        return -1;
    }

    int absolute = (path[0] == '/');
    const char *p = absolute ? path : strip_dotslash(path);
    size_t plen = strlen(p);

    int best = -1;
    size_t best_len = 0;

    for (int i = 0; i < count; i++) {
        const struct preopen *e = &entries[i];
        size_t elen = e->prefix_len;

        /* Treat empty prefix or "." as a universal match for *relative*
         * paths.  Absolute paths must match an absolute prefix. */
        int is_universal = (elen == 0)
            || (elen == 1 && e->prefix[0] == '.')
            || (elen == 2 && e->prefix[0] == '.' && e->prefix[1] == '/');

        if (is_universal) {
            if (absolute)
                continue;
            if (best < 0) {
                best = i;
                best_len = 0;
            }
            continue;
        }

        /* For absolute paths only consider absolute prefixes; same for
         * relative. */
        int prefix_absolute = (elen > 0 && e->prefix[0] == '/');
        if (absolute != prefix_absolute)
            continue;

        if (elen > plen)
            continue;
        if (memcmp(p, e->prefix, elen) != 0)
            continue;
        /* Boundary: either the entire path equals the prefix, or the
         * next character in the path is '/'. */
        if (elen < plen && p[elen] != '/')
            continue;

        if (best < 0 || elen > best_len) {
            best = i;
            best_len = elen;
        }
    }

    if (best < 0) {
        errno = ENOENT;
        return -1;
    }

    *out_basefd = entries[best].fd;

    /* Skip over the matched prefix and any separating slashes. */
    const char *rel = p + best_len;
    while (*rel == '/')
        rel++;

    /* path_open with an empty path returns the directory itself; if the
     * relative is empty we pass "." instead to be safe. */
    if (*rel == '\0') {
        *out_rel = ".";
        *out_rel_len = 1;
    } else {
        *out_rel = rel;
        *out_rel_len = strlen(rel);
    }
    return 0;
}
