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

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <map>
#include <string>
#include <vector>

namespace mrc {

class Resource {
public:
  Resource(const std::string &type_, int quantity_);
  virtual ~Resource();

  std::string type;
  int quantity;
};

typedef std::vector<Resource> ResourceList;
typedef std::vector<ResourceList> ResourceListOptions;
void concatResourceList(ResourceList *dst, const ResourceList *src);
void concatResourceListOptions(ResourceListOptions *dst, const ResourceListOptions *src);

} // namespace mrc

#endif // RESOURCE_H_
