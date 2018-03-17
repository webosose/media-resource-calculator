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

void* setup() {
  ResourceCalculator *calculator = ResourceCalculator::create();
  should_be(calculator != 0);

  return calculator;
}

void teardown(void *fxtr) {
  delete (ResourceCalculator*)fxtr;
}

void caseDefault(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceList list = calculator->calcAdecResources(
      ResourceCalculator::kAudioEtc, 4, 2);
  should_be_eq(list.size(), 1);

  const Resource &res = list[0];
  should_be(res.type == "ADEC");
  should_be_eq(res.quantity, 1);
}

void caseAAC6(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceList list = calculator->calcAdecResources(
      ResourceCalculator::kAudioMPEG, 4, 6);
  should_be_eq(list.size(), 2);

  int adecQuantity = 0;
  int notSupportedQuantity = 0;

  for (int i = 0; i < list.size(); i++) {
    const Resource &res = list[i];
    if (res.type == "ADEC") {
      adecQuantity += res.quantity;
    } else if (res.type == "NOTSUPPORTED") {
      notSupportedQuantity += res.quantity;
    }
  }

  should_be_eq(adecQuantity, 1);
  should_be_eq(notSupportedQuantity, 1);
}

void caseDTS(void *fxtr) {
  ResourceCalculator *calculator = (ResourceCalculator*)fxtr;

  ResourceList list = calculator->calcAdecResources(
      ResourceCalculator::kAudioDTS, 4, 2);
  should_be_eq(list.size(), 1);

  const Resource &res = list[0];
  should_be(res.type == "ADEC");
  should_be_eq(res.quantity, 2);
}

int main(int argc, char* argv[]) {
  int ret;

  should_suite_t *s;

  s = should_create_suite("resource_calculator for raspberrypi3 adec");
  if (!s) {
    return false;
  }

  should_set_fixture(s, setup, teardown);

  should_add_case(s, caseDefault);
  should_add_case(s, caseAAC6);
  should_add_case(s, caseDTS);

  ret = should_run_and_destroy_suite(s);

  return ret;
}
