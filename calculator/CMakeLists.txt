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

message(STATUS "BUILDING calculator")

set(MEDIA-RESOURCE-CALCULATOR_SRC
    resource.cpp
    resource_table.cpp
    resource_calculator.cpp
    resource_calculator_c.cpp
    video_resource_table.cpp
    )

set(MEDIA-RESOURCE-CALCULATOR_HEADER
    resource.h
    resource_table.h
    resource_calculator.h
    resource_calculator_c.h
    video_resource_table.h
    )

set(LIBS
    ${LIBPBNJSON_LDFLAGS})

if(${WEBOS_TARGET_MACHINE} STREQUAL "raspberrypi3" OR ${WEBOS_TARGET_MACHINE} STREQUAL "raspberrypi3-64")
  message("Build resource calculator for raspberrypi3")

  add_definitions(-DPLATFORM_RASPBERRYPI3)

  set(ADEC_RESOURCE_JSON ../data/raspberrypi3_adec.json)
  set(VDEC_RESOURCE_JSON ../data/raspberrypi3_vdec.json)
  set(ADEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/adec_resources_raspberrypi3.h)
  set(VDEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/vdec_resources_raspberrypi3.h)

elseif(${WEBOS_TARGET_MACHINE} MATCHES "qemux86")
  message("Build resource calculator for qemux86")

  add_definitions(-DPLATFORM_QEMUX86)

  set(ADEC_RESOURCE_JSON ../data/qemux86_adec.json)
  set(VDEC_RESOURCE_JSON ../data/qemux86_vdec.json)
  set(ADEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/adec_resources_qemux86.h)
  set(VDEC_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/vdec_resources_qemux86.h)

else()
  message("Check the resource calculator json files")
endif()

# Generate resource headers from resource json files
GENERATE_RESOURCE_HEADER(ADEC_RESOURCE
                         ${ADEC_RESOURCE_JSON}
                         ${ADEC_RESOURCE_HEADER})
GENERATE_RESOURCE_HEADER(VDEC_RESOURCE
                         ${VDEC_RESOURCE_JSON}
                         ${VDEC_RESOURCE_HEADER})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(media-resource-calculator SHARED
            ${MEDIA-RESOURCE-CALCULATOR_SRC}
            ${MEDIA-RESOURCE-CALCULATOR_HEADER}
            ${ADEC_RESOURCE_HEADER}
            ${VDEC_RESOURCE_HEADER}
            ${VDEC_RESOURCE_HEADER_A0A1}
            )

target_link_libraries(media-resource-calculator
                      ${LIBS}
                      )

set_target_properties(media-resource-calculator PROPERTIES
                      VERSION ${MEDIA-RESOURCE-CALCULATOR_VERSION}
                      SOVERSION ${MEDIA-RESOURCE-CALCULATOR_VERSION_MAJOR}
                      )

configure_file(media-resource-calculator.pc.in ${CMAKE_CURRENT_BINARY_DIR}/media-resource-calculator.pc @ONLY)

install(TARGETS media-resource-calculator LIBRARY DESTINATION lib)
install(FILES ${MEDIA-RESOURCE-CALCULATOR_HEADER} DESTINATION "include/media-resource-calculator")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/media-resource-calculator.pc
        DESTINATION "lib/pkgconfig")
