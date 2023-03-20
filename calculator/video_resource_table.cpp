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

#include "video_resource_table.h"

#include <assert.h>
#include <pbnjson.hpp>
#include <sstream>
#include <stdlib.h>

namespace pj = pbnjson;

namespace mrc {

using std::string;

VideoInfo::VideoInfo(std::string videoInfoString)
    : width(0)
    , height(0)
    , frameRate(0) {
  std::string token;
  std::istringstream iss(videoInfoString);
  while (std::getline(iss, token, ' ')) {
    size_t delim;
    if ((delim = token.find('*')) != std::string::npos) {
      width = atoi(token.substr(0, delim).c_str());
      height = atoi(token.substr(delim + 1, std::string::npos).c_str());
    } else if (token.find("2k") != std::string::npos ||
        token.find("2K") != std::string::npos) {
      width = 2048;
      height = 1080;
    } else if (token.find("4k") != std::string::npos ||
        token.find("4K") != std::string::npos) {
      width = 4096;
      height = 2160;
    } else if ((delim = token.find('p')) != std::string::npos ||
        (delim = token.find('P')) != std::string::npos) {
      frameRate = atoi(token.substr(0, delim).c_str());
    } else if ((delim = token.find('i')) != std::string::npos ||
        (delim = token.find('I')) != std::string::npos) {
      frameRate = atoi(token.substr(0, delim).c_str()) / 2;
    } else {
      // unhandled expressions should be notified
      assert(0);
    }
  }
}

VideoResourceTable::VideoResourceTable() {}

VideoResourceTable::~VideoResourceTable() {}

static ResourceList createResourceList(const pj::JValue &data) {
  assert(data.isObject());

  ResourceList ret;

  pj::JValue::ObjectIterator it;
  for (it = data.children().begin(); it != data.children().end(); it++) {
    if (!(*it).first.isString() || !(*it).second.isNumber()) continue;

    ret.push_back(Resource((*it).first.asString(), (*it).second.asNumber<int32_t>()));
  }

  return ret;
}

static ResourceListOptions createResourceListOptions(const pj::JValue &data) {
  ResourceListOptions ret;

  if (data.isObject()) {
    ret.push_back(createResourceList(data));
  } else if (data.isArray()) {
    pj::JValue::ArrayIterator it;
    for (it = data.items().begin(); it != data.items().end(); it++) {
      if (!(*it).isObject()) continue;

      ret.push_back(createResourceList(*it));
    }
  }

  return ret;
}

static CodecTable createCodecTable(const pj::JValue &data) {
  assert(data.isObject());

  CodecTable ret;

  pj::JValue::ObjectIterator it;
  for (it = data.children().begin(); it != data.children().end(); it++) {
    if (!(*it).first.isString()) continue;

    ResourcePair pair(
        VideoInfo((*it).first.asString()),
        createResourceListOptions((*it).second));

    CodecTable::iterator ic;
    for (ic = ret.begin(); ic != ret.end(); ic++) {
      if (pair.first.isSubsetOf(ic->first)) {
        ret.insert(ic, pair);
        break;
      }
    }

    if (ic == ret.end()) {
      ret.push_back(pair);
    }
  }

  return ret;
}

bool VideoResourceTable::setData(const std::string &data) {
  pj::JSchemaFragment inputSchema("{}");

  pj::JDomParser parser;
  if (!parser.parse(data, inputSchema, NULL)) {
    return false;
  }

  pj::JValue parsed = parser.getDom();

  if (!parsed.isObject()) {
    return false;
  }

  resourceTable.clear();

  pj::JValue::ObjectIterator it;
  for (it = parsed.children().begin(); it != parsed.children().end(); it++) {
    if (!(*it).first.isString() || !(*it).second.isObject()) {
        return false;
    }

    resourceTable.insert(std::pair<std::string, CodecTable>(
                        (*it).first.asString(),
                        createCodecTable((*it).second)));
  }

  return true;
}

const ResourceListOptions* VideoResourceTable::lookup(
    const std::string codec, int width, int height, int frameRate) const {
  std::map<std::string, CodecTable>::const_iterator it;

  it = resourceTable.find(codec);
  if (it == resourceTable.end()) {
    it = resourceTable.find("default");
    if (it == resourceTable.end()) {
      return 0;
    }
  }

  VideoInfo videoInfo(width, height, frameRate);
  CodecTable::const_iterator ic;
  for (ic = it->second.begin(); ic != it->second.end(); ic++) {
    if (videoInfo.isSubsetOf(ic->first)) {
      return static_cast<const ResourceListOptions*>(&ic->second);
    }
  }

  return 0;
}

} // namespace mrc
