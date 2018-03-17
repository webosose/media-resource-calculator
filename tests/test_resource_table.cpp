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

#include "should/should.h"

#include <calculator/resource_table.h>

static const char* TEST_RESOURCE =
"{"
"  \"H.264\": {"
"    \"MSVC\": 1,"
"    \"bandwidth\": 2"
"  },"
"  \"MPEG2\": {"
"    \"G1\": 1"
"  },"
"  \"VP9\": {"
"    \"G2\": 1,"
"    \"bandwidth\": 2"
"  }"
"}";

using mrc::ResourceTable;
using mrc::Resource;
using mrc::ResourceList;

void* setup() {
  ResourceTable *table = new ResourceTable();

  should_be(table->setData(TEST_RESOURCE));

  return table;
}

void teardown(void *fxtr) {
  delete (ResourceTable*)fxtr;
}

void basicCase(void *fxtr) {
  ResourceTable *table = (ResourceTable*)fxtr;

  const ResourceList *list;

  list = table->lookup("MPEG21");
  should_be(list == 0);

  list = table->lookup("MPEG2");
  should_be(list != 0);
  should_be(list->size() == 1);

  const Resource &res = list->at(0);
  should_be(res.type == "G1");
  should_be(res.quantity == 1);
}

void twoResourcesCase(void *fxtr) {
  ResourceTable *table = (ResourceTable*)fxtr;

  const ResourceList *list;

  list = table->lookup("VP9");
  should_be(list != 0);
  should_be(list->size() == 2);

  int g2Quantity = 0;
  int bandwidthQuantity = 0;

  for (int i = 0; i < list->size(); i++) {
    const Resource &res = list->at(i);
    if (res.type == "G2") {
      g2Quantity += res.quantity;
    } else if (res.type == "bandwidth") {
      bandwidthQuantity += res.quantity;
    }
  }

  should_be(g2Quantity == 1);
  should_be(bandwidthQuantity == 2);
}

void twoCandidatesCase(void *fxtr) {
  ResourceTable *table = (ResourceTable*)fxtr;

  const ResourceList *list;

  list = table->lookup("H.264");
  should_be(list != 0);
  should_be(list->size() == 2);

  int g1Quantity = 0;
  int bandwidthQuantity = 0;

  for (int i = 0; i < list->size(); i++) {
    const Resource &res = list->at(i);
    if (res.type == "G1") {
      g1Quantity += res.quantity;
    } else if (res.type == "bandwidth") {
      bandwidthQuantity += res.quantity;
    }
  }

  should_be(g1Quantity == 0);
  should_be(bandwidthQuantity == 2);

  int msvcQuantity = 0;
  bandwidthQuantity = 0;

  for (int i = 0; i < list->size(); i++) {
    const Resource &res = list->at(i);
    if (res.type == "MSVC") {
      msvcQuantity += res.quantity;
    } else if (res.type == "bandwidth") {
      bandwidthQuantity += res.quantity;
    }
  }

  should_be(msvcQuantity == 1);
  should_be(bandwidthQuantity == 2);
}

int main(int argc, char* argv[]) {
  int ret;

  should_suite_t *s;

  s = should_create_suite("resource_table");
  if (!s) {
    return false;
  }

  should_set_fixture(s, setup, teardown);

  should_add_case(s, basicCase);
  should_add_case(s, twoResourcesCase);
  should_add_case(s, twoCandidatesCase);

  ret = should_run_and_destroy_suite(s);

  return ret;
}
