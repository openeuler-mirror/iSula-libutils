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

cmake_minimum_required (VERSION 2.8)
project (isula_libutils)

include(cmake/checker.cmake)

option(VERSION "set isula_libutils version" ON)
if (VERSION STREQUAL "ON")
    set(ISULA_LIBUTILS_VERSION "1.0.8")
endif()

option(DEBUG "set isula_libutils gcc option" ON)
if (DEBUG STREQUAL "ON")
    add_definitions("-g -o2")
endif()

option(GCOV "set isula_libutils gcov option" OFF)
if (GCOV STREQUAL "ON")
    set(ISULA_LIBUTILS_GCOV "ON")
endif()

set(CMAKE_C_COMPILER "gcc" CACHE PATH "c compiler")
set(CMAKE_C_FLAGS "-fPIC -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -Wall -Werror")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-E -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wtrampolines -fPIE -pie -shared -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_BINARY_DIR}/conf/config.h"
)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/isula_libutils.pc.in"
	"${CMAKE_BINARY_DIR}/conf/isula_libutils.pc"
)

set(LIBTYPE "SHARED")

if (LIB_INSTALL_DIR)
  set(LIB_INSTALL_DIR_DEFAULT ${LIB_INSTALL_DIR})
else()
  set(LIB_INSTALL_DIR_DEFAULT "lib")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)

# install all files
install(FILES ${CMAKE_BINARY_DIR}/conf/isula_libutils.pc
	DESTINATION ${LIB_INSTALL_DIR_DEFAULT}/pkgconfig PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE)

install(FILES third_party/log.h DESTINATION include/isula_libutils)
install(FILES third_party/libocispec/read_file.h DESTINATION include/isula_libutils)
install(FILES src/constants.h DESTINATION include/isula_libutils)
install(FILES src/utils.h DESTINATION include/isula_libutils)
install(FILES src/json/oci_runtime_hooks.h DESTINATION include/isula_libutils)
install(FILES src/json/parse_common.h DESTINATION include/isula_libutils)
install(DIRECTORY ${CMAKE_BINARY_DIR}/json/ DESTINATION include/isula_libutils
        FILES_MATCHING PATTERN "*.h")

