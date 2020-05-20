/******************************************************************************
 * iSula-libutils: utils library for iSula
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 *
 * Authors:
 * Haozi007 <liuhao27@huawei.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ********************************************************************************/

#include "utils.h"

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

#include "log.h"
#include "constants.h"

#define ISSLASH(C) ((C) == '/')
#define IS_ABSOLUTE_FILE_NAME(F) (ISSLASH ((F)[0]))
#define IS_RELATIVE_FILE_NAME(F) (!IS_ABSOLUTE_FILE_NAME (F))

void *isula_libutils_smart_calloc_s(size_t unit_size, size_t count)
{
    if (unit_size == 0) {
        return NULL;
    }

    if (count > (MAX_MEMORY_SIZE / unit_size)) {
        return NULL;
    }

    return calloc(count, unit_size);
}

void *isula_libutils_common_calloc_s(size_t size)
{
    if (size == 0 || size > MAX_MEMORY_SIZE) {
        return NULL;
    }

    return calloc((size_t)1, size);
}

char *isula_libutils_strdup_s(const char *src)
{
    char *dst = NULL;

    if (src == NULL) {
        return NULL;
    }

    dst = strdup(src);
    if (dst == NULL) {
        abort();
    }

    return dst;
}

ssize_t isula_libutils_read_nointr(int fd, void *buf, size_t count)
{
    ssize_t rn = 0;
    bool empty_buf = (buf == NULL || count == 0);

    if (fd == -1) {
        return -1;
    }
    if (empty_buf) {
        return 0;
    }
    for (;;) {
        rn = read(fd, buf, count);
        if (rn < 0 && errno == EINTR) {
            continue;
        }
        break;
    }

    return rn;
}

ssize_t isula_libutils_write_nointr(int fd, const void *buf, size_t count)
{
    ssize_t nret;

    if (buf == NULL) {
        return -1;
    }

    for (;;) {
        nret = write(fd, buf, count);
        if (nret < 0 && errno == EINTR) {
            continue;
        } else {
            break;
        }
    }
    return nret;
}

bool isula_libutils_dir_exists(const char *path)
{
    struct stat s;
    int nret;

    if (path == NULL) {
        return false;
    }

    nret = stat(path, &s);
    if (nret < 0) {
        return false;
    }

    return S_ISDIR(s.st_mode);
}

bool util_file_exists(const char *f)
{
    struct stat buf;
    int nret;

    if (f == NULL) {
        return false;
    }

    nret = stat(f, &buf);
    if (nret < 0) {
        return false;
    }
    return true;
}

static bool do_clean_path_continue(const char *endpos, const char *stpos, const char *respath, char **dst)
{
    if (endpos - stpos == 1 && stpos[0] == '.') {
        return true;
    } else if (endpos - stpos == 2 && stpos[0] == '.' && stpos[1] == '.') {
        char *dest = *dst;
        if (dest <= respath + 1) {
            return true;
        }
        for (--dest; dest > respath && !ISSLASH(dest[-1]); --dest) {
            continue;
        }
        *dst = dest;
        return true;
    }
    return false;
}

static int do_clean_path(const char *respath, const char *limit_respath,
                         const char *stpos, char **dst)
{
    char *dest = *dst;
    const char *endpos = NULL;

    for (endpos = stpos; *stpos; stpos = endpos) {
        while (ISSLASH(*stpos)) {
            ++stpos;
        }

        for (endpos = stpos; *endpos && !ISSLASH(*endpos); ++endpos) {
        }

        if (endpos - stpos == 0) {
            break;
        } else if (do_clean_path_continue(endpos, stpos, respath, &dest)) {
            continue;
        }

        if (!ISSLASH(dest[-1])) {
            *dest++ = '/';
        }

        if (dest + (endpos - stpos) >= limit_respath) {
            ERROR("Path is too long");
            if (dest > respath + 1) {
                dest--;
            }
            *dest = '\0';
            return -1;
        }

        (void)memcpy(dest, stpos, (size_t)(endpos - stpos));
        dest += endpos - stpos;
        *dest = '\0';
    }
    *dst = dest;
    return 0;
}

char *cleanpath(const char *path, char *realpath, size_t realpath_len)
{
    char *respath = NULL;
    char *dest = NULL;
    const char *stpos = NULL;
    const char *limit_respath = NULL;

    if (path == NULL || path[0] == '\0' || \
        realpath == NULL || (realpath_len < PATH_MAX)) {
        return NULL;
    }

    respath = realpath;

    (void)memset(respath, 0, realpath_len);
    limit_respath = respath + PATH_MAX;

    if (!IS_ABSOLUTE_FILE_NAME(path)) {
        if (!getcwd(respath, PATH_MAX)) {
            ERROR("Failed to getcwd");
            respath[0] = '\0';
            goto error;
        }
        dest = strchr(respath, '\0');
        if (dest == NULL) {
            ERROR("Failed to get the end of respath");
            goto error;
        }
        if (strlen(path) >= (PATH_MAX - 1) - strlen(respath)) {
            ERROR("%s path too long", path);
            goto error;
        }
        (void)strcat(respath, path);
        stpos = path;
    } else {
        dest = respath;
        *dest++ = '/';
        stpos = path;
    }

    if (do_clean_path(respath, limit_respath, stpos, &dest)) {
        goto error;
    }

    if (dest > respath + 1 && ISSLASH(dest[-1])) {
        --dest;
    }
    *dest = '\0';

    return respath;

error:
    return NULL;
}

int isula_libutils_open(const char *filename, int flags, mode_t mode)
{
    char rpath[PATH_MAX] = { 0x00 };

    if (cleanpath(filename, rpath, sizeof(rpath)) == NULL) {
        return -1;
    }
    if (mode) {
        return open(rpath, flags | O_CLOEXEC, mode);
    } else {
        return open(rpath, flags | O_CLOEXEC);
    }
}

static void set_char_to_terminator(char *p)
{
    *p = '\0';
}

/*
 * @name is absolute path of this file.
 * make all directory in this absolute path.
 */
int isula_libutils_build_dir(const char *name)
{
    char *n = NULL;
    char *p = NULL;
    char *e = NULL;
    int nret;

    if (name == NULL) {
        return -1;
    }

    n = isula_libutils_strdup_s(name);

    if (n == NULL) {
        ERROR("Out of memory while creating directory '%s'.", name);
        return -1;
    }

    e = &n[strlen(n)];
    for (p = n + 1; p < e; p++) {
        if (*p != '/') {
            continue;
        }
        set_char_to_terminator(p);
        nret = mkdir(n, ISULA_LIBUTILS_DIRECTORY_MODE);
        if (nret && (errno != EEXIST || !isula_libutils_dir_exists(n))) {
            ERROR("failed to create directory '%s'.", n);
            free(n);
            return -1;
        }
        *p = '/';
    }
    free(n);
    return 0;
}

