/*
 * Copyright (c) 2008-2021 LG Electronics, Inc.
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

#include "resource_calculator.h"

#include <algorithm>
#include <assert.h>
#include <cmath>

#ifdef PLATFORM_RASPBERRYPI3
#include "adec_resources_raspberrypi3.h"
#include "vdec_resources_raspberrypi3.h"
#include "venc_resources_raspberrypi3.h"
#include "disp_resources_raspberrypi3.h"

#elif PLATFORM_RASPBERRYPI4
#include "adec_resources_raspberrypi4.h"
#include "vdec_resources_raspberrypi4.h"
#include "venc_resources_raspberrypi4.h"
#include "disp_resources_raspberrypi4.h"

#elif PLATFORM_QEMUX86
#include "adec_resources_qemux86.h"
#include "vdec_resources_qemux86.h"
#include "venc_resources_qemux86.h"
#include "disp_resources_qemux86.h"

#else
#  error "platform is not specified"
#endif

namespace mrc {

ResourceCalculator::ResourceCalculator() {
  adecTable.setData(ADEC_RESOURCE);
  vdecTable.setData(VDEC_RESOURCE);
  vencTable.setData(VENC_RESOURCE);
  displayPlanesTable.setData(DISP_RESOURCE);
}

ResourceCalculator::~ResourceCalculator() {}

ResourceCalculator* ResourceCalculator::create() {
  return new ResourceCalculator();
}

std::string ResourceCalculator::videoCodecToString(VideoCodec codec) const
{
  switch (codec) {
  case kVideoEtc:
    return "default";
  case kVideoH264:
    return "H264";
  case kVideoH265:
    return "H265";
  case kVideoMPEG:
    return "MPEG";
  case kVideoMVC:
    return "MVC";
  case kVideoSVC:
    return "SVC";
  case kVideoVP9:
    return "VP9";
  case kVideoRM:
    return "RM";
  case kVideoAVS:
    return "AVS";
  case kVideoVP8:
    return "VP8";
  case kVideoMJPEG:
    return "MJPEG";
  case kVideoMPEG4:
    return "MPEG4";
  case kVideoJPEG:
    return "JPEG";
  default:
    return "default";
  }
}

ResourceList ResourceCalculator::calcVdecResources(
    VideoCodecs codecs,
    int width,
    int height,
    int frameRate,
    ScanType scanType,
    _3DType _3dType) {
  ResourceList vdec;
  ResourceList notSupported;
  notSupported.push_back(Resource("NOTSUPPORTED", 1));

  if (scanType == kScanInterlaced) frameRate /= 2;
  if (_3dType != k3DNone) frameRate *= 2;

  for (int i = 0; codecs >> i != 0; i++) {
    int codec = 1 << i;
    if (!(codec & codecs)) continue;

    const ResourceListOptions *options = vdecTable.lookup(
        videoCodecToString(static_cast<VideoCodec>(codec)),
        width,
        height,
        frameRate);

    if (!options) {
      return notSupported;
    }

    concatResourceList(&vdec, &*options->begin());
  }

  return vdec;
}

ResourceListOptions ResourceCalculator::calcVdecResourceOptions(
    VideoCodecs codecs,
    int width,
    int height,
    int frameRate,
    ScanType scanType,
    _3DType _3dType) {
  ResourceListOptions vdec;
  ResourceList notSupported;
  notSupported.push_back(Resource("NOTSUPPORTED", 1));

  if (scanType == kScanInterlaced) frameRate /= 2;
  if (_3dType != k3DNone) frameRate *= 2;

  for (int i = 0; codecs >> i != 0; i++) {
    int codec = 1 << i;
    if (!(codec & codecs)) continue;

    const ResourceListOptions *options = vdecTable.lookup(
        videoCodecToString(static_cast<VideoCodec>(codec)),
        width,
        height,
        frameRate);

    if (!options) {
      vdec.clear();
      vdec.push_back(notSupported);
      return vdec;
    }

    concatResourceListOptions(&vdec, options);
  }

  return vdec;
}

ResourceListOptions ResourceCalculator::calcVencResourceOptions(
    VideoCodecs codecs,
    int width,
    int height,
    int frameRate) {
  ResourceListOptions venc;
  ResourceList notSupported;
  notSupported.push_back(Resource("NOTSUPPORTED", 1));

  for (int i = 0; codecs >> i != 0; i++) {
    int codec = 1 << i;
    if (!(codec & codecs)) continue;

    const ResourceListOptions *options = vencTable.lookup(
        videoCodecToString(static_cast<VideoCodec>(codec)),
        width,
        height,
        frameRate);

    if (!options) {
      venc.clear();
      venc.push_back(notSupported);
      return venc;
    }

    concatResourceListOptions(&venc, options);
  }

  return venc;
}

ResourceList ResourceCalculator::calcAdecResources(
    AudioCodecs codecs,
    int version,
    int channel) {
  ResourceList adec;

  if (codecs & kAudioMPEG &&
      channel == 6 && (version == 4 || version == 2)) {
    concatResourceList(&adec, adecTable.lookup("aac6"));
  }

  if (codecs & kAudioPCM) {
    concatResourceList(&adec, adecTable.lookup("pcm"));
  }

  if (codecs & kAudioDTS) {
    concatResourceList(&adec, adecTable.lookup("dts"));
  }

  if (codecs & kAudioDTSE) {
    concatResourceList(&adec, adecTable.lookup("dtse"));
  }

  if (codecs & kAudioMPEGH) {
    concatResourceList(&adec, adecTable.lookup("mpeg-h"));
  }

  if (codecs & kAudioAC4) {
    concatResourceList(&adec, adecTable.lookup("ac4"));
  }

  if (codecs & kAudioATMOS) {
    concatResourceList(&adec, adecTable.lookup("atmos"));
  }

  if (codecs & kAudioDescription) {
    concatResourceList(&adec, adecTable.lookup("description"));
  }

  if (adec.size() == 0 || codecs & kAudioEtc) {
    concatResourceList(&adec, adecTable.lookup("default"));
  }

  assert(0 < adec.size());

  return adec;
}

const std::string kResourceCPB = "SVP_CPB";
const std::string kResourceRB = "SVP_RB";

ResourceList ResourceCalculator::calcMiscResources(
    bool useSecureVideoPath, bool useSecureReformatter) {
  ResourceList misc;

  if (useSecureVideoPath) {
    misc.push_back(Resource(kResourceCPB, 2));
  }
  return misc;
}

const std::string kResourceDISP = "DISP";

ResourceListOptions ResourceCalculator::calcDisplayPlaneResourceOptions(
    RenderMode renderMode)
{
  ResourceListOptions disp;
  ResourceList notSupported;
  notSupported.push_back(Resource("NOTSUPPORTED", 1));

  switch (renderMode) {
    case kModePunchThrough:
      // for punch through mode, get the resources needed for
      // the display resource (could be a list of planes out of
      // which any one plane will do)
      {
        const ResourceListOptions *options = displayPlanesTable.lookup(kResourceDISP);
        if (!options) {
          disp.clear();
          disp.push_back(notSupported);
        } else {
          concatResourceListOptions(&disp, options);
        }
      }
      break;
    case kModeTexture:
      // texture mode does not need any display planes
      // nothing to do here
      break;
    default:
      // this is an unexpected value for the renderMode
      // so we just err out
      {
        disp.clear();
        disp.push_back(notSupported);
      }
      break;
  }

  return disp;
}

} // namespace mrc
