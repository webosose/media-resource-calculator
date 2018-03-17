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

#include "resource.h"

#include <assert.h>

namespace mrc {

Resource::Resource(const std::string &type_, int quantity_)
    : type(type_),
      quantity(quantity_) {}

Resource::~Resource() {}

void concatResourceList(ResourceList *dst, const ResourceList *src) {
  assert(dst);

  if (src == 0 || src->size() == 0) return;

  if (dst->size() == 0) {
    *dst = *src;
    return;
  }

  for (ResourceList::const_iterator is = src->begin(); is != src->end(); ++is) {
    ResourceList::iterator id;
    for (id = dst->begin(); id != dst->end(); ++id) {
      if (0 == id->type.compare(is->type)) {
        if (id->quantity < is->quantity) {
          id->quantity = is->quantity;
        }
        break;
      }
    }
    if (id == dst->end()) {
      dst->push_back(*is);
    }
  }
}

void concatResourceListOptions(ResourceListOptions *dst,
                               const ResourceListOptions *src) {
  if (src == 0 || src->size() == 0) return;

  if (dst->size() == 0) {
    *dst = *src;
    return;
  }

  ResourceListOptions newOptions;
  ResourceListOptions::const_iterator i, j;
  for (i = dst->begin(); i != dst->end(); i++) {
    for (j = src->begin(); j != src->end(); j++) {
      ResourceList res = *i;
      concatResourceList(&res, &*j);
      newOptions.push_back(res);
    }
  }

  *dst = newOptions;
}

} // namespace mrc
