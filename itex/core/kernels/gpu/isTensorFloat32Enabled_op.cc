/* Copyright (c) 2021-2023 Intel Corporation

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

#include "itex/core/kernels/gpu/isTensorFloat32Enabled_op.h"

#include <string>

#include "itex/core/kernels/common/fill_functor.h"
#include "itex/core/utils/bcast.h"
#include "itex/core/utils/errors.h"
#include "itex/core/utils/op_kernel.h"
#include "itex/core/utils/op_requires.h"
#include "itex/core/utils/register_types.h"
#include "itex/core/utils/types.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"

namespace itex {
// An op that returns True if TensorFloat-32 execution is enabled. Useful for
// testing that enabling/disabling TensorFloat-32 works correctly, even when
// the test does not run with a GPU that supports TensorFloat-32.

typedef Eigen::GpuDevice GPUDevice;

template <typename Device, typename T>
class IsTensorFloat32Enabled : public OpKernel {
 public:
  explicit IsTensorFloat32Enabled(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    Tensor* output = nullptr;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(0, TensorShape({}), &output));
    output->scalar<T>()() = tensor_float_32_execution_enabled();
  }
};

#define REGISTER_ISTENSORFLOAT32ENABLED_GPU(T)           \
  REGISTER_KERNEL_BUILDER(Name("IsTensorFloat32Enabled") \
                              .Device(DEVICE_GPU)        \
                              .TypeConstraint<T>("T")    \
                              .HostMemory("output"),     \
                          IsTensorFloat32Enabled<GPUDevice, T>)

TF_CALL_bool(REGISTER_ISTENSORFLOAT32ENABLED_GPU);
#undef REGISTER_ISTENSORFLOAT32ENABLED_GPU
}  // namespace itex
