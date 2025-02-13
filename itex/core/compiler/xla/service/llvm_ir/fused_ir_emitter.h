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

#ifndef ITEX_CORE_COMPILER_XLA_SERVICE_LLVM_IR_FUSED_IR_EMITTER_H_
#define ITEX_CORE_COMPILER_XLA_SERVICE_LLVM_IR_FUSED_IR_EMITTER_H_

#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "itex/core/compiler/xla/service/elemental_ir_emitter.h"
#include "itex/core/compiler/xla/service/hlo_instruction.h"
#include "itex/core/compiler/xla/statusor.h"
#include "llvm/IR/Value.h"

namespace itex_xla {

// FusedIrEmitter is used to generate code for fusion nodes.
//
// Unlike IrEmitter and its ilk, which directly create LLVM IR in an LLVM
// Module, FusedIrEmitter is better understood as "IR generator generator".
// FusedIrEmitter recursively creates a generator (a host function) which the
// compiler can invoke at a later time.  Invoking the generator emits LLVM IR
// that, when run, produces the value at a particular index of the output.
//
// After building this generator, the compiler creates a loop (or its moral
// equivalent, e.g. a GPU kernel) and calls the generator from within the loop.
// This generates code that produces each element of the output.
//
// This class handles both vanilla fusion and multi-output fusion.  In the MOF
// case, the fusion node ends with a kTuple instruction, and the generator
// created produces an LLVM struct with N elements, one for each element of the
// arrays in the tuple.  It follows that the arrays in the tuple must have the
// same length.
class FusedIrEmitter {
 public:
  using IndexedGenerator = llvm_ir::ElementGenerator;

  explicit FusedIrEmitter(ElementalIrEmitter& elemental_emitter)
      : elemental_emitter_(elemental_emitter) {}

  void BindGenerator(const HloInstruction& instruction,
                     llvm_ir::ElementGenerator generator) {
    indexed_generators_[&instruction] = std::move(generator);
  }

  // Returns the generator function for the given instruction.
  StatusOr<IndexedGenerator> GetGenerator(const HloInstruction& instruction);

  // Evaluates whether fusing 'producer' into 'consumer' might cause exponential
  // behavior in FusedIrEmitter. We currently can have exponential time/memory
  // requirements for emitting certain fusion kernels, in which case we don't
  // want to fuse.
  // TODO(b/119692968): Remove this once we have fixed our fusion emitter.
  static bool IsFusedIrEmitterInefficient(const HloInstruction& consumer,
                                          const HloInstruction& producer);

 private:
  StatusOr<IndexedGenerator> CreateGenerator(const HloInstruction& instruction);
  StatusOr<IndexedGenerator> DefaultAction(const HloInstruction& instruction);
  IndexedGenerator HandleConstant(const HloInstruction& constant);
  StatusOr<IndexedGenerator> HandleTuple(const HloInstruction& tuple);

  ElementalIrEmitter& elemental_emitter_;

  // Map from instructions to functions that generate code for the output
  // elements. If an instruction is a GetTupleElement instruction, the
  // instruction produces non-tuple result.
  absl::flat_hash_map<const HloInstruction*, IndexedGenerator>
      indexed_generators_;

  // Cache of generated values, lest we regenerate an element of a node with
  // multiple outgoing edges.
  // Use instruction and index values as the key.
  using ValueCacheKey =
      std::pair<const HloInstruction*, std::vector<llvm::Value*>>;
  absl::flat_hash_map<ValueCacheKey, llvm::Value*> value_cache_;
};

}  // namespace itex_xla

#endif  // ITEX_CORE_COMPILER_XLA_SERVICE_LLVM_IR_FUSED_IR_EMITTER_H_
