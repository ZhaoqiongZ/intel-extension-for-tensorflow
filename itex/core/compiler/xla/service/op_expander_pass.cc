/* Copyright (c) 2023 Intel Corporation

Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "itex/core/compiler/xla/service/op_expander_pass.h"

#include <utility>
#include <vector>

#include "absl/algorithm/container.h"
#include "itex/core/compiler/xla/service/hlo_creation_utils.h"
#include "itex/core/compiler/xla/service/hlo_instruction.h"
#include "itex/core/compiler/xla/statusor.h"
#include "itex/core/compiler/xla/util.h"

namespace itex_xla {

StatusOr<bool> OpExpanderPass::Run(
    HloModule* module,
    const absl::flat_hash_set<absl::string_view>& execution_threads) {
  std::vector<HloInstruction*> matching_instructions;
  for (HloComputation* computation :
       module->MakeNonfusionComputations(execution_threads)) {
    absl::c_copy_if(computation->instructions(),
                    std::back_inserter(matching_instructions),
                    [&](HloInstruction* inst) {
                      return InstructionMatchesPattern(inst) &&
                             (!extra_filter_ || extra_filter_(inst));
                    });
  }

  for (HloInstruction* inst : matching_instructions) {
    TF_ASSIGN_OR_RETURN(HloInstruction * expanded_root,
                        ExpandInstruction(inst));
    if (expanded_root == nullptr) {
      continue;
    }
    TF_RETURN_IF_ERROR(inst->parent()->ReplaceInstruction(inst, expanded_root));
  }

  return !matching_instructions.empty();
}
}  // namespace itex_xla
