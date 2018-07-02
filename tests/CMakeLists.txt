# Copyright (c) 2008-2018 LG Electronics, Inc.
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

message(STATUS "BUILDING tests")

include_directories(${MEDIA-RESOURCE-CALCULATOR_SOURCE_DIR})

# tests for ResourceTable
add_executable(
  test_resource_table
  test_resource_table.cpp
  should/should.c
  ../calculator/resource.cpp
  ../calculator/resource_table.cpp
)

target_link_libraries(test_resource_table ${LIBPBNJSON_LDFLAGS})
add_test(test_resource_table test_resource_table)

# tests for VideoResourceTable
add_executable(
  test_video_resource_table
  test_video_resource_table.cpp
  should/should.c
  ../calculator/resource.cpp
  ../calculator/video_resource_table.cpp
)

target_link_libraries(test_video_resource_table ${LIBPBNJSON_LDFLAGS})
add_test(test_video_resource_table test_video_resource_table)

# tests for ResourceCalculator
macro(add_calculator_testcase testname testfile platform platform_flag)
  set(ADEC_RESOURCE_JSON ../data/${platform}_adec.json)
  set(VDEC_RESOURCE_JSON ../data/${platform}_vdec.json)
  set(DISP_RESOURCE_JSON ../data/${platform}_disp.json)
  set(ADEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/adec_resources_${platform}.h)
  set(VDEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/vdec_resources_${platform}.h)
  set(DISP_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/disp_resources_${platform}.h)

  GENERATE_RESOURCE_HEADER(ADEC_RESOURCE
                           ${ADEC_RESOURCE_JSON}
                           ${ADEC_RESOURCE_HEADER})
  GENERATE_RESOURCE_HEADER(VDEC_RESOURCE
                           ${VDEC_RESOURCE_JSON}
                           ${VDEC_RESOURCE_HEADER})
  GENERATE_RESOURCE_HEADER(DISP_RESOURCE
                           ${DISP_RESOURCE_JSON}
                           ${DISP_RESOURCE_HEADER})

  add_executable(${testname}
                 ${testfile}
                 ../calculator/resource.cpp
                 ../calculator/resource.h
                 ../calculator/resource_calculator.cpp
                 ../calculator/resource_calculator_c.cpp
                 ../calculator/resource_calculator.h
                 ../calculator/resource_table.cpp
                 ../calculator/resource_table.h
                 ../calculator/video_resource_table.cpp
                 ../calculator/video_resource_table.h
                 ../calculator/display_resource_table.cpp
                 ../calculator/display_resource_table.h
                 ${ADEC_RESOURCE_HEADER}
                 ${VDEC_RESOURCE_HEADER}
                 should/should.c)
  SET_TARGET_PROPERTIES(${testname} PROPERTIES COMPILE_FLAGS -D${platform_flag})
  target_link_libraries(${testname} ${LIBPBNJSON_LDFLAGS})
  add_test(${testname} ${testname})
endmacro()

add_calculator_testcase(test_raspberrypi3_adec test_raspberrypi3_adec.cpp raspberrypi3 PLATFORM_RASPBERRYPI3)

add_calculator_testcase(test_raspberrypi3_vdec test_raspberrypi3_vdec.cpp raspberrypi3 PLATFORM_RASPBERRYPI3)

# TODO: Add test case for disp resource
# add_calculator_testcase(test_raspberrypi3_disp test_raspberrypi3_disp.cpp raspberrypi3 PLATFORM_RASPBERRYPI3)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../calculator ${CMAKE_CURRENT_BINARY_DIR}/../tests)

