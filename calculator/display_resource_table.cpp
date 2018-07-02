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

#include "display_resource_table.h"

#include <assert.h>
#include <pbnjson.hpp>

namespace pj = pbnjson;

namespace mrc {

using std::string;

DisplayResourceTable::DisplayResourceTable() {}

DisplayResourceTable::~DisplayResourceTable() {}

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

bool DisplayResourceTable::setData(const std::string &data) {
  pj::JSchemaFragment inputSchema("{}");

  pj::JDomParser parser;
  if (!parser.parse(data, inputSchema, NULL)) {
    return false;
  }

  pj::JValue parsed = parser.getDom();

  if (!parsed.isObject()) {
    return false;
  }

  resourceData.clear();

  pj::JValue::ObjectIterator it;
  for (it = parsed.begin(); it != parsed.end(); it++) {
    if (!(*it).first.isString() || !(*it).second.isObject()) return false;

    resourceData.insert(std::pair<std::string, ResourceListOptions>(
                        (*it).first.asString(),
                        createResourceListOptions((*it).second)));
  }

  return true;
}

const ResourceListOptions* DisplayResourceTable::lookup(
    const std::string &key) const {
  std::map<std::string, ResourceListOptions>::const_iterator it;

  it = resourceData.find(key);

  if (it == resourceData.end()) {
    return 0;
  } else {
    return static_cast<const ResourceListOptions*>(&it->second);
  }
}

} // namespace mrc
