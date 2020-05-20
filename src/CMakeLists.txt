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

# generate .c and .h files for json
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/json)

# get json source files
aux_source_directory(${CMAKE_BINARY_DIR}/json generatesrcs)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/json topjsonsrcs)

# get libisula_libutils source files
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} topsrcs)

# set libisula_libutils library
add_library(isula_libutils
    ${LIBTYPE}
    ${topsrcs}
    ${THIRD_PARTY_SRCS}
    ${generatesrcs}
    ${commonjsonsrcs}
    ${topjsonsrcs}
    )

# set include dirs
#PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils
target_include_directories(isula_libutils
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_BINARY_DIR}/json
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/json/schema/src
        PUBLIC ${THIRD_PARTY_INCS}
        PUBLIC ${ZLIB_INCLUDE_DIR}
        )

# set liblcr compile flags
if (ISULA_LIBUTILS_GCOV)
    set(CMAKE_C_FLAGS_DEBUG "-Wall -fprofile-arcs -ftest-coverage")
    message("------compile with gcov-------------")
    message("-----CFLAGS: " ${CMAKE_C_FLAGS_DEBUG})
    message("------------------------------------")
    target_link_libraries(isula_libutils -lgcov)
endif()

target_link_libraries(isula_libutils ${ZLIB_LIBRARY} ${LIBYAJL_LIBRARY})

# install all files
install(TARGETS isula_libutils
	LIBRARY DESTINATION ${LIB_INSTALL_DIR_DEFAULT} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
