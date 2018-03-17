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

#include <calculator/resource_calculator.h>

using mrc::ResourceCalculator;
using mrc::Resource;
using mrc::ResourceList;
using mrc::ResourceListOptions;

void* setup() {
  ResourceCalculator *calculator = ResourceCalculator::create();
  should_be(calculator != 0);

  return calculator;
}

void teardown(void *fxtr) {
  delete (ResourceCalculator*)fxtr;
}

static int getResourceQuantity(const ResourceList &list, std::string type) {
  int quantity = 0;
  for (unsigned int i = 0; i < list.size(); i++) {
    const Resource &res = list[i];
    if (res.type == type) {
      quantity += res.quantity;
    }
  }
  return quantity;
}

void case_Etc_2K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoEtc, 1920, 1088, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_Etc_2K_40P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoEtc, 1920, 1088, 40,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 4);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_Etc_2K_40P_2(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoEtc, 1445, 1445, 40,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 4);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H264_2K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH264, 2048, 1088, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H264_2K_60P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH264, 2048, 1088, 60,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H264_4K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH264, 4096, 2304, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 4);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 4);
}

void case_H265_2K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH265, 2048, 1088, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H265_2K_60P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH265, 2048, 1088, 60,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H265_2K_60P_2(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH265, 1088, 2048, 60,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_H265_4K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH265, 4096, 2176, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 2);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 4);
}

void case_H265_4K_60P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoH265, 4096, 2176, 60,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "HEVC"), 4);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 2);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 4);
}

void case_VP9_2K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoVP9, 2048, 1088, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_VP9_2K_60P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoVP9, 2048, 1088, 60,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 2);
}

void case_VP9_4K_30P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoVP9, 4096, 2304, 30,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 3);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 4);
}

void case_VP9_4K_50P(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoVP9, 4096, 2160, 50,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "G2"), 4);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 2);
  should_be_eq(getResourceQuantity(options[0], "VDEC_BUS"), 4);
}

void case_JPEG_4K(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoJPEG, 3840, 2160, 0,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "IMG_DEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
}

void case_JPEG_4K_2(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceListOptions options = calculator->calcVdecResourceOptions(
      ResourceCalculator::kVideoJPEG, 2160, 3840, 0,
      ResourceCalculator::kScanProgressive,
      ResourceCalculator::k3DNone);
  should_be_eq(options.size(), 1);

  should_be_eq(getResourceQuantity(options[0], "IMG_DEC"), 1);
  should_be_eq(getResourceQuantity(options[0], "VDEC"), 1);
}

int main(int argc, char* argv[]) {
  int ret;

  should_suite_t *s;

  s = should_create_suite("resource_calculator for raspberrypi3 vdec");
  if (!s) {
    return false;
  }

  should_set_fixture(s, setup, teardown);

  should_add_case(s, case_Etc_2K_30P);
  should_add_case(s, case_Etc_2K_40P);
  should_add_case(s, case_Etc_2K_40P_2);

  should_add_case(s, case_H264_2K_30P);
  should_add_case(s, case_H264_2K_60P);
  should_add_case(s, case_H264_4K_30P);

  should_add_case(s, case_H265_2K_30P);
  should_add_case(s, case_H265_2K_60P);
  should_add_case(s, case_H265_2K_60P_2);
  should_add_case(s, case_H265_4K_30P);
  should_add_case(s, case_H265_4K_60P);

  should_add_case(s, case_VP9_2K_30P);
  should_add_case(s, case_VP9_2K_60P);
  should_add_case(s, case_VP9_4K_30P);
  should_add_case(s, case_VP9_4K_50P);

  should_add_case(s, case_JPEG_4K);
  should_add_case(s, case_JPEG_4K_2);

  ret = should_run_and_destroy_suite(s);

  return ret;
}
