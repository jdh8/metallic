#ifndef METALLIC_WASI_STAT_CONVERT_H
#define METALLIC_WASI_STAT_CONVERT_H

#include <sys/stat.h>
#include "wasi.h"

static inline void wasi_fill_stat(struct stat *dst, const __wasi_filestat_t *src)
{
    dst->st_dev     = src->dev;
    dst->st_ino     = src->ino;
    dst->st_nlink   = src->nlink;
    dst->st_uid     = 0;
    dst->st_gid     = 0;
    dst->st_rdev    = 0;
    dst->st_size    = (off_t)src->size;
    dst->st_blksize = 4096;
    dst->st_blocks  = (blkcnt_t)((src->size + 511) / 512);

    dst->st_atim.tv_sec  = (time_t)(src->atim / 1000000000u);
    dst->st_atim.tv_nsec = (long)(src->atim % 1000000000u);
    dst->st_mtim.tv_sec  = (time_t)(src->mtim / 1000000000u);
    dst->st_mtim.tv_nsec = (long)(src->mtim % 1000000000u);
    dst->st_ctim.tv_sec  = (time_t)(src->ctim / 1000000000u);
    dst->st_ctim.tv_nsec = (long)(src->ctim % 1000000000u);

    switch (src->filetype) {
    case __WASI_FILETYPE_REGULAR_FILE:     dst->st_mode = S_IFREG  | 0644; break;
    case __WASI_FILETYPE_DIRECTORY:        dst->st_mode = S_IFDIR  | 0755; break;
    case __WASI_FILETYPE_SYMBOLIC_LINK:    dst->st_mode = S_IFLNK  | 0777; break;
    case __WASI_FILETYPE_CHARACTER_DEVICE: dst->st_mode = S_IFCHR  | 0666; break;
    case __WASI_FILETYPE_BLOCK_DEVICE:     dst->st_mode = S_IFBLK  | 0666; break;
    case __WASI_FILETYPE_SOCKET_DGRAM:
    case __WASI_FILETYPE_SOCKET_STREAM:    dst->st_mode = S_IFSOCK | 0666; break;
    default:                               dst->st_mode = 0; break;
    }
}

#endif /* METALLIC_WASI_STAT_CONVERT_H */
