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

#ifndef VIDEO_RESOURCE_TABLE_H_
#define VIDEO_RESOURCE_TABLE_H_

#include "resource.h"
#include <list>
#include <string>

namespace mrc {

struct VideoInfo {
  int width;
  int height;
  int frameRate;

  VideoInfo(int width_,
            int height_,
            int frameRate_)
      : width(width_)
      , height(height_)
      , frameRate(frameRate_) { };

  VideoInfo(std::string videoInfoString);

  // true if this is subset of info
  bool isSubsetOf(const VideoInfo &info) {
    return width * height <= info.width * info.height &&
        frameRate <= info.frameRate;
  }
};

typedef std::pair<VideoInfo, ResourceListOptions> ResourcePair;
typedef std::list<ResourcePair> CodecTable;

class VideoResourceTable {
public:
  VideoResourceTable();
  virtual ~VideoResourceTable();

  bool setData(const std::string &data);
  const ResourceListOptions* lookup(std::string codec,
                             int width,
                             int height,
                             int frameRate) const;

private:
  std::map<std::string, CodecTable> resourceTable;
};

} // namespace mrc

#endif // VIDEO_RESOURCE_TABLE_H_
