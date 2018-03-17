/*
 * Copyright (c) 2008-2018 LG Electronics, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0



 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 * SPDX-License-Identifier: Apache-2.0
 */

#include "resource_calculator_c.h"

#include <malloc.h>
#include "resource_calculator.h"
#include <string.h>

static MRCResource *createResourceFromCpp(mrc::Resource &cppResource) {
  MRCResource *resource = (MRCResource *)malloc(sizeof(MRCResource));
  if (resource) {
    resource->type = strdup(cppResource.type.c_str());
    resource->quantity = cppResource.quantity;
  }

  return resource;
}

static void deleteResource(MRCResource *resource) {
  free(resource->type);
  free(resource);
}

static MRCResourceList createResourceListFromCpp(
    mrc::ResourceList &cppResourceList) {
  if (cppResourceList.size() == 0) {
    return 0;
  }

  MRCResourceList resourceList = (MRCResource**)
      malloc(sizeof(MRCResource*) * (cppResourceList.size() + 1));
  if (resourceList) {
    for (int i = 0; i < cppResourceList.size(); i++) {
      resourceList[i] = createResourceFromCpp(cppResourceList[i]);
    }
    resourceList[cppResourceList.size()] = 0;
  }

  return resourceList;
}

void MRCDeleteResourceList(MRCResourceList resourceList) {
  if (!resourceList) {
    return;
  }

  for (int i = 0; resourceList[i] != 0; i++) {
    deleteResource(resourceList[i]);
  }

  free(resourceList);
}

static MRCResourceListOptions createResourceListOptionsFromCpp(
    mrc::ResourceListOptions &cppResourceListOptions) {
  if (cppResourceListOptions.size() == 0) {
    return 0;
  }

  MRCResourceListOptions options = (MRCResourceList*)
      malloc(sizeof(MRCResourceList) * (cppResourceListOptions.size() + 1));
  if (options) {
    for (int i = 0; i < cppResourceListOptions.size(); i++) {
      options[i] = createResourceListFromCpp(cppResourceListOptions[i]);
    }
    options[cppResourceListOptions.size()] = 0;
  }

  return options;
}

void MRCDeleteResourceListOptions(MRCResourceListOptions options) {
  if (!options) {
    return;
  }

  for (int i = 0; options[i] != 0; i++) {
    MRCDeleteResourceList(options[i]);
  }

  free(options);
}

static mrc::ResourceCalculator *calc = 0;

// codecs are combination of bitmasks defined in enum VideoCodec.
// Zero width or height is considered as 4K resolution.
MRCResourceList MRCCalcVdecResources(int codecs,
                                     int width,
                                     int height,
                                     int frameRate,
                                     MRCScanType scanType,
                                     MRC3DType _3dType) {
  if (!calc) {
    calc = mrc::ResourceCalculator::create();
  }

  mrc::ResourceList resourceList = calc->calcVdecResources(
      codecs, width, height, frameRate,
      static_cast<mrc::ResourceCalculator::ScanType>(static_cast<int>(scanType)),
      static_cast<mrc::ResourceCalculator::_3DType>(static_cast<int>(_3dType)));

  MRCResourceList list = createResourceListFromCpp(resourceList);

  return list;
}

MRCResourceListOptions MRCCalcVdecResourceOptions(int codecs,
                                                  int width,
                                                  int height,
                                                  int frameRate,
                                                  enum MRCScanType scanType,
                                                  MRC3DType _3dType) {
  if (!calc) {
    calc = mrc::ResourceCalculator::create();
  }

  mrc::ResourceListOptions resourceListOptions =
      calc->calcVdecResourceOptions(codecs, width, height, frameRate,
          static_cast<mrc::ResourceCalculator::ScanType>(static_cast<int>(scanType)),
          static_cast<mrc::ResourceCalculator::_3DType>(static_cast<int>(_3dType)));

  MRCResourceListOptions options =
      createResourceListOptionsFromCpp(resourceListOptions);

  return options;
}

MRCResourceList MRCCalcAdecResources(int codecs,
                                     int version,
                                     int channel) {
  if (!calc) {
    calc = mrc::ResourceCalculator::create();
  }

  mrc::ResourceList resourceList = calc->calcAdecResources(
      codecs, version, channel);

  MRCResourceList list = createResourceListFromCpp(resourceList);

  return list;
}

MRCResourceList MRCCalcMiscResources(int useSecureVideoPath, int useSecureReformatter) {
  if (!calc) {
    calc = mrc::ResourceCalculator::create();
  }

  mrc::ResourceList resourceList = calc->calcMiscResources(
      useSecureVideoPath, useSecureReformatter);

  MRCResourceList list = createResourceListFromCpp(resourceList);

  return list;
}
