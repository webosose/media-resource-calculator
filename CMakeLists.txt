# Copyright (c) 2008-2024 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0



# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 2.8.7)

project(MEDIA-RESOURCE-CALCULATOR)

include(webOS/webOS)
webos_modules_init(1 1 1 QUALIFIER RC6)

include(FindPkgConfig)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/data")
include(ConfMacros)

set(MEDIA-RESOURCE-CALCULATOR_VERSION_MAJOR 1)
set(MEDIA-RESOURCE-CALCULATOR_VERSION_MINOR 0)
set(MEDIA-RESOURCE-CALCULATOR_VERSION_PATCH 0)
set(MEDIA-RESOURCE-CALCULATOR_VERSION
    "${MEDIA-RESOURCE-CALCULATOR_VERSION_MAJOR}"
    ".${MEDIA-RESOURCE-CALCULATOR_VERSION_MINOR}"
    ".${MEDIA-RESOURCE-CALCULATOR_VERSION_PATCH}")

pkg_check_modules(LIBPBNJSON REQUIRED pbnjson_cpp)
webos_add_compiler_flags(ALL ${LIBPBNJSON_CFLAGS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c99")

add_subdirectory(calculator)

if(NOT NO_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()
