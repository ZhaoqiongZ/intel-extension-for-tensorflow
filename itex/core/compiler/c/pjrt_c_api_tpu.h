/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef ITEX_CORE_COMPILER_C_PJRT_C_API_TPU_H_
#define ITEX_CORE_COMPILER_C_PJRT_C_API_TPU_H_

#include "itex/core/compiler/c/pjrt_c_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO(b/261916900): this will cause symbol collision when we wanna statically
// link in multiple plugins. We may need to make the function name configurable,
// or we could add external-only wrapper shims
// Does not pass ownership of returned PJRT_Api* to caller.
const PJRT_Api* GetPjrtApi();

#ifdef __cplusplus
}
#endif

#endif  // ITEX_CORE_COMPILER_C_PJRT_C_API_TPU_H_
