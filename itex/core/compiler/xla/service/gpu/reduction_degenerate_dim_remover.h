/* Copyright (c) 2023 Intel Corporation

Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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
#ifndef ITEX_CORE_COMPILER_XLA_SERVICE_GPU_REDUCTION_DEGENERATE_DIM_REMOVER_H_
#define ITEX_CORE_COMPILER_XLA_SERVICE_GPU_REDUCTION_DEGENERATE_DIM_REMOVER_H_

#include "absl/types/optional.h"
#include "itex/core/compiler/xla/service/hlo_instructions.h"
#include "itex/core/compiler/xla/service/hlo_module.h"
#include "itex/core/compiler/xla/service/hlo_pass_interface.h"

namespace itex_xla {
namespace gpu {

// Enforces the invariant that reduction input and output have no degenerate
// (size 1) dimension. Since these dimensions are physically meaningless, they
// are removed using bitcasts.
//
// For example,
//
//   f[1] out = reduce(f[100, 1, 1] input, dimensions={0, 1})
//
// becomes:
//
//
//   f[100] tmp1 = f[100] bitcast(f[100, 1, 1], input)
//   f[] tmp2 = reduce(f[100] tmp1, dimensions={0})
//   f[1] out = f[] bitcast(tmp2)
//
class ReductionDegenerateDimRemover : public HloModulePass {
 public:
  absl::string_view name() const override {
    return "reduction-degenerate-dim-remover";
  }

  using HloPassInterface::Run;
  StatusOr<bool> Run(
      HloModule* module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) override;
};

}  // namespace gpu
}  // namespace itex_xla

#endif  // ITEX_CORE_COMPILER_XLA_SERVICE_GPU_REDUCTION_DEGENERATE_DIM_REMOVER_H_
