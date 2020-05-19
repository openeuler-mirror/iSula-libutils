# iSula-libutils: utils library for iSula
#
# Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
#
# Authors:
# Haozi007 <liuhao27@huawei.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

# check depends library and headers
find_package(PkgConfig)

macro(_CHECK)
if (${ARGV0} STREQUAL "${ARGV1}")
	message("error: can not find " ${ARGV2} " program")
	set(CHECKER_RESULT 1)
else()
	message("--  found " ${ARGV2} " --- works")
endif()
endmacro()


# check zlib
pkg_check_modules(PC_ZLIB "zlib>=1.2.8")
find_path(ZLIB_INCLUDE_DIR zlib.h
    HINTS ${PC_ZLIB_INCLUDEDIR} ${PC_ZLIB_INCLUDE_DIRS})
_CHECK(ZLIB_INCLUDE_DIR "ZLIB_INCLUDE_DIR-NOTFOUND" "zlib.h")
find_library(ZLIB_LIBRARY z
  HINTS ${PC_ZLIB_LIBDIR} ${PC_ZLIB_LIBRARY_DIRS})
_CHECK(ZLIB_LIBRARY "ZLIB_LIBRARY-NOTFOUND" "libz.so")

#check python3
find_program(CMD_PYTHON python3)
_CHECK(CMD_PYTHON "CMD_PYTHON-NOTFOUND" "python3")

# check libyajl
pkg_check_modules(PC_LIBYAJL REQUIRED "yajl>=2")
if (NOT PC_LIBYAJL_FOUND)
	message("error: can not find yajl>=2")
	set(CHECKER_RESULT 1)
endif()
find_path(LIBYAJL_INCLUDE_DIR yajl/yajl_tree.h
	HINTS ${PC_LIBYAJL_INCLUDEDIR} ${PC_LIBYAJL_INCLUDE_DIRS})
_CHECK(LIBYAJL_INCLUDE_DIR "LIBYAJL_INCLUDE_DIR-NOTFOUND" "yajl/yajl_tree.h")

find_library(LIBYAJL_LIBRARY yajl
	HINTS ${PC_LIBYAJL_LIBDIR} ${PC_LIBYAJL_LIBRARY_DIRS})
_CHECK(LIBYAJL_LIBRARY "LIBYAJL_LIBRARY-NOTFOUND" "libyajl.so")

if (ENABLE_TESTS STREQUAL "ON")
	pkg_check_modules(PC_CHECK REQUIRED "check>=0.9.12")
	if (NOT PC_CHECK_FOUND)
		message("error: can not find check>=0.9.12")
		set(CHECKER_RESULT 1)
	endif()
endif()
