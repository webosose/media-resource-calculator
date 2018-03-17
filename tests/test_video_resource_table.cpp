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

#include <calculator/video_resource_table.h>

static const char* TEST_RESOURCE =
"{"
"  \"H.264\": {"
"    \"1920*1080 30p\": { \"BODA\": 1, \"VDEC\": 1},"
"    \"3840*2160 30p\": { \"G1\": 4, \"VDEC\": 2 },"
"    \"3840*2160 120p\": { \"G1\": 8, \"VDEC\": 2 },"
"    \"1920*1080 60p\": { \"BODA\": 2, \"VDEC\": 1 }"
"  },"
"  \"VP9\": {"
"    \"4K 30p\": { \"G2\": 4 },"
"    \"2K 60p\": { \"G2\": 2 },"
"    \"2k 30p\": { \"G2\": 1 }"
"  },"
"  \"H.265\": {"
"    \"1920*1080 30p\": [{ \"IP1\": 2, \"VDEC\": 1},"
"                        { \"IP2\": 2, \"VDEC\": 1}]"
"  },"
"  \"MPEG\": {"
"    \"1920*1080 30p\": { \"IP1\": 2, \"VDEC\": 1}"
"  },"
"  \"default\": {"
"    \"2048*1080 30p\": { \"BODA\": 1 }"
"  }"
"}";

using mrc::VideoResourceTable;
using mrc::Resource;
using mrc::ResourceList;
using mrc::ResourceListOptions;

void* setup() {
  VideoResourceTable *table = new VideoResourceTable();

  should_be(table->setData(TEST_RESOURCE));

  return table;
}

void teardown(void *fxtr) {
  delete (VideoResourceTable*)fxtr;
}

static int getResourceQuantity(const ResourceList &list, std::string type)
{
  int quantity = 0;
  for (unsigned int i = 0; i < list.size(); i++) {
    const Resource &res = list[i];
    if (res.type == type) {
      quantity += res.quantity;
    }
  }
  return quantity;
}

void ordered1(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 1920, 1080, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();
  should_be_eq(getResourceQuantity(*list, "BODA"), 1);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 1);
}

void ordered2(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 1920, 1080, 60);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "BODA"), 2);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 1);
}

void ordered3(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 3840, 2160, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G1"), 4);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 2);
}

void ordered4(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 3840, 2160, 100);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G1"), 8);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 2);
}

void ordered5(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 1080, 1920, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "BODA"), 1);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 1);
}

void ordered6(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 2160, 3840, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G1"), 4);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 2);
}

void ordered_notfound(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 3840, 2160, 180);
  should_be(options == 0);
}

void unordered1(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("VP9", 2048, 1080, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G2"), 1);
}

void unordered2(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("VP9", 2048, 1080, 60);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G2"), 2);
}

void unordered3(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("VP9", 4096, 2160, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G2"), 4);
}

void unordered4(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("VP9", 1080, 2048, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G2"), 1);
}

void unordered_notfound(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("VP9", 4096, 2160, 60);
  should_be(options == 0);
}

void default_found(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("TOAST", 2048, 1080, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "BODA"), 1);
}

void default_notfound(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("TOAST", 4096, 2160, 60);
  should_be(options == 0);
}

void nonboundary_found1(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 720, 480, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "BODA"), 1);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 1);
}

void nonboundary_found2(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 1920, 1080, 50);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "BODA"), 2);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 1);
}

void nonboundary_found3(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.264", 3000, 1500, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();

  should_be_eq(getResourceQuantity(*list, "G1"), 4);
  should_be_eq(getResourceQuantity(*list, "VDEC"), 2);
}

void nonboundary_notfound(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options;

  options = table->lookup("H.264", 5000, 3000, 30);
  should_be(options == 0);
}

void or_1(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  const ResourceListOptions *options = table->lookup("H.265", 1920, 1080, 30);
  should_be(options != 0);
  const ResourceList *list = &options->front();
  should_be_eq(getResourceQuantity((*options)[0], "IP1"), 2);
  should_be_eq(getResourceQuantity((*options)[1], "IP2"), 2);
}

void or_2(void *fxtr) {
  VideoResourceTable *table = (VideoResourceTable *)fxtr;

  ResourceListOptions options1 = *table->lookup("H.265", 1920, 1080, 30);
  ResourceListOptions options2 = *table->lookup("MPEG", 1920, 1080, 30);
  should_be(options1.size() != 0);
  should_be(options2.size() != 0);

  concatResourceListOptions(&options1, &options2);
  should_be_eq(getResourceQuantity(options1[0], "IP1"), 2);

  should_be_eq(getResourceQuantity(options1[1], "IP1"), 2);
  should_be_eq(getResourceQuantity(options1[1], "IP2"), 2);
}

int main(int argc, char* argv[]) {
  int ret;

  should_suite_t *s;

  s = should_create_suite("video_resource_table");
  if (!s) {
    return false;
  }

  should_set_fixture(s, setup, teardown);

  should_add_case(s, ordered1);
  should_add_case(s, ordered2);
  should_add_case(s, ordered3);
  should_add_case(s, ordered4);
  should_add_case(s, ordered5);
  should_add_case(s, ordered6);
  should_add_case(s, ordered_notfound);

  should_add_case(s, unordered1);
  should_add_case(s, unordered2);
  should_add_case(s, unordered3);
  should_add_case(s, unordered4);
  should_add_case(s, unordered_notfound);

  should_add_case(s, default_found);
  should_add_case(s, default_notfound);

  should_add_case(s, nonboundary_found1);
  should_add_case(s, nonboundary_found2);
  should_add_case(s, nonboundary_found3);
  should_add_case(s, nonboundary_notfound);

  should_add_case(s, or_1);
  should_add_case(s, or_2);

  ret = should_run_and_destroy_suite(s);

  return ret;
}
