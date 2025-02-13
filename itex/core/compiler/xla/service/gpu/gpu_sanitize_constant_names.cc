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

#include "itex/core/compiler/xla/service/gpu/gpu_sanitize_constant_names.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "itex/core/compiler/xla/service/hlo_instruction.h"
#include "itex/core/compiler/xla/service/hlo_opcode.h"
#include "itex/core/compiler/xla/service/llvm_ir/buffer_assignment_util.h"
#include "itex/core/utils/logging.h"
#include "itex/core/utils/status.h"

namespace itex_xla {

namespace gpu {

StatusOr<bool> GpuSanitizeConstantNames::Run(
    HloModule* module,
    const absl::flat_hash_set<absl::string_view>& execution_threads) {
  bool changed = false;

  NameUniquer instr_name_uniquer(/*separator=*/"_");
  // Collect the names used for the non-constant HLO instructions.+
  for (HloComputation* computation : module->computations(execution_threads)) {
    for (HloInstruction* instr : computation->instructions()) {
      if (instr->opcode() == HloOpcode::kConstant) {
        continue;
      }

      const std::string& old_name = instr->name();
      instr->UniquifyName(&instr_name_uniquer);
      ITEX_CHECK_EQ(old_name, instr->name());
    }
  }

  // Sanitize the names for the constant HLO instructions and make them unique.
  // This is not merged into the above loop because we don't want this pass to
  // change the names of non-constant instructions, that is, if a constant HLO
  // conflicts with a non-constant HLO, we change the name of the constant HLO
  // even though the non-constant HLO comes after in the HLO module.
  for (HloComputation* computation : module->computations(execution_threads)) {
    for (HloInstruction* instr : computation->instructions()) {
      if (instr->opcode() != HloOpcode::kConstant) {
        continue;
      }
      std::string sanitized_name = llvm_ir::SanitizeConstantName(*instr);
      instr->SetAndSanitizeName(sanitized_name);
      instr->UniquifyName(&instr_name_uniquer);
      changed = true;
    }
  }

  return changed;
}  // namespace gpu

}  // namespace gpu
}  // namespace itex_xla
