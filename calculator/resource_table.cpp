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

#include "resource_table.h"

#include <assert.h>
#include <pbnjson.hpp>

namespace pj = pbnjson;

namespace mrc {

using std::string;

ResourceTable::ResourceTable() {}

ResourceTable::~ResourceTable() {}

static ResourceList createResourceList(const pj::JValue &data) {
  assert(data.isObject());

  ResourceList ret;

  pj::JValue::ObjectIterator it;
  for (it = data.begin(); it != data.end(); it++) {
    if (!(*it).first.isString() || !(*it).second.isNumber()) continue;

    ret.push_back(Resource((*it).first.asString(), (*it).second.asNumber<int32_t>()));
  }

  return ret;
}

bool ResourceTable::setData(const std::string &data) {
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

    resourceData.insert(std::pair<std::string, ResourceList>(
                        (*it).first.asString(),
                        createResourceList((*it).second)));
  }

  return true;
}

const ResourceList* ResourceTable::lookup(
    const std::string &key) const {
  std::map<std::string, ResourceList>::const_iterator it;

  it = resourceData.find(key);

  if (it == resourceData.end()) {
    return 0;
  } else {
    return static_cast<const ResourceList*>(&it->second);
  }
}

} // namespace mrc
