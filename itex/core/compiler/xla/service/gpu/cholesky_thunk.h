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

#ifndef ITEX_CORE_COMPILER_XLA_SERVICE_GPU_CHOLESKY_THUNK_H_
#define ITEX_CORE_COMPILER_XLA_SERVICE_GPU_CHOLESKY_THUNK_H_
#if ITEX_USE_MKL

#include "absl/types/optional.h"
#include "itex/core/compiler/xla/service/buffer_assignment.h"
#include "itex/core/compiler/xla/service/gpu/buffer_allocations.h"
#include "itex/core/compiler/xla/service/gpu/gpu_executable.h"
#include "itex/core/compiler/xla/service/gpu/thunk.h"
#include "itex/core/compiler/xla/service/hlo_instruction.h"
#include "itex/core/compiler/xla/types.h"
#include "itex/core/utils/status.h"
#include "mkl.h"  // NOLINT(build/include_subdir)
#include "oneapi/mkl/lapack.hpp"
#include "protos/xla_data.pb.h"

namespace itex_xla {
namespace gpu {

// This class stores everything that StreamExecutor needs to launch a Cholesky
// decomposition (LAPACK potrf). It is generated by IrEmitter.
//
// As an implementation detail, we may run potrf (potentially in a loop, if
// batch_size >1), or potrfBatched.
//
// Thread-compatible.
class CholeskyThunk : public Thunk {
 public:
  CholeskyThunk(ThunkInfo thunk_info, const CholeskyOptions& options,
                BufferAllocation::Slice a_buffer,
                BufferAllocation::Slice workspace_buffer,
                BufferAllocation::Slice info_buffer, PrimitiveType type,
                int64_t batch_size, int64_t n);

  CholeskyThunk(const CholeskyThunk&) = delete;
  CholeskyThunk& operator=(const CholeskyThunk&) = delete;

  Status ExecuteOnStream(const ExecuteParams& params) override;

 private:
  template <typename T>
  Status DoPotrfBatched(const ExecuteParams& params);

  const oneapi::mkl::uplo uplo_;

  const BufferAllocation::Slice a_buffer_;
  const BufferAllocation::Slice workspace_buffer_;

  const PrimitiveType type_;
  const int64_t batch_size_;
  const int64_t n_;
};

}  // namespace gpu
}  // namespace itex_xla

#endif  // ITEX_USE_MKL
#endif  // ITEX_CORE_COMPILER_XLA_SERVICE_GPU_CHOLESKY_THUNK_H_
