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

#ifndef __ISULA_LIBUTILS_UTILS_H
#define __ISULA_LIBUTILS_UTILS_H

#include <sys/types.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __WORDSIZE == 64
// current max user memory for 64-machine is 2^47 B
#define MAX_MEMORY_SIZE ((size_t)1 << 47)
#else
// current max user memory for 32-machine is 2^31 B
#define MAX_MEMORY_SIZE ((size_t)1 << 31)
#endif

void *isula_libutils_smart_calloc_s(size_t unit_size, size_t count);
void *isula_libutils_common_calloc_s(size_t size);
char *isula_libutils_strdup_s(const char *src);
ssize_t isula_libutils_read_nointr(int fd, void *buf, size_t count);
ssize_t isula_libutils_write_nointr(int fd, const void *buf, size_t count);
bool isula_libutils_dir_exists(const char *path);
bool util_file_exists(const char *f);
int isula_libutils_open(const char *filename, int flags, mode_t mode);
int isula_libutils_build_dir(const char *name);

#ifdef __cplusplus
}
#endif

#endif
