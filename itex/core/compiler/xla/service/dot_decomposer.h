/* Copyright (c) 2023 Intel Corporation

Copyright 2017 The TensorFlow Authors. All Rights Reserved.

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

#ifndef ITEX_CORE_COMPILER_XLA_SERVICE_DOT_DECOMPOSER_H_
#define ITEX_CORE_COMPILER_XLA_SERVICE_DOT_DECOMPOSER_H_

#include "itex/core/compiler/xla/service/hlo_module.h"
#include "itex/core/compiler/xla/service/hlo_pass_interface.h"

namespace itex_xla {

// DotDecomposer is a pass which converts dots into a canonical form where
// non-contracting and contracting dimensions are reshaped together and batch
// dimensions are the most major dimensions.
class DotDecomposer : public HloModulePass {
 public:
  absl::string_view name() const override { return "dot_decomposer"; }

  // Run DotDecomposer pass on computations in 'module'.
  // Returns whether the 'module' was changed.
  using HloPassInterface::Run;
  StatusOr<bool> Run(
      HloModule* module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) override;
};

}  // namespace itex_xla

#endif  // ITEX_CORE_COMPILER_XLA_SERVICE_DOT_DECOMPOSER_H_
