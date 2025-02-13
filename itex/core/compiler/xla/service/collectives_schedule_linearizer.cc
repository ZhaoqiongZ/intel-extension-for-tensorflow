/* Copyright (c) 2023 Intel Corporation

Copyright 2021 The TensorFlow Authors. All Rights Reserved.

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

#include "itex/core/compiler/xla/service/collectives_schedule_linearizer.h"

#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_join.h"
#include "itex/core/compiler/xla/literal.h"
#include "itex/core/compiler/xla/service/hlo_casting_utils.h"
#include "itex/core/compiler/xla/service/hlo_domain_map.h"
#include "itex/core/compiler/xla/service/hlo_instruction.h"
#include "itex/core/compiler/xla/service/hlo_instructions.h"
#include "itex/core/compiler/xla/service/hlo_opcode.h"
#include "itex/core/compiler/xla/service/hlo_query.h"
#include "itex/core/compiler/xla/service/hlo_reachability.h"
#include "itex/core/compiler/xla/service/shape_inference.h"
#include "itex/core/compiler/xla/shape_util.h"
#include "itex/core/compiler/xla/status_macros.h"
#include "itex/core/utils/errors.h"
#include "protos/xla_data.pb.h"

namespace itex_xla {

// TODO(b/181653482): Fix for interprocedural collectives as well.
StatusOr<bool> CollectivesScheduleLinearizer::Run(
    HloModule* module,
    const absl::flat_hash_set<absl::string_view>& execution_threads) {
  bool changed = false;
  for (HloComputation* computation :
       module->MakeNonfusionComputations(execution_threads)) {
    std::unique_ptr<HloReachabilityMap> reachability =
        HloReachabilityMap::Build(computation);
    HloCollectiveInstruction* prev = nullptr;
    for (HloInstruction* instruction :
         computation->MakeInstructionPostOrder()) {
      if (auto* next = DynCast<HloCollectiveInstruction>(instruction)) {
        if (prev != nullptr && !reachability->IsConnected(next, prev)) {
          // If prev and next are independent, enforce ordering.
          TF_RETURN_IF_ERROR(prev->AddControlDependencyTo(next));
          ITEX_VLOG(1) << "Adding control dependency from " << prev->ToString()
                       << " to " << next->ToString();
          changed = true;
        }
        prev = next;
      }
    }
  }
  return changed;
}

}  // namespace itex_xla
