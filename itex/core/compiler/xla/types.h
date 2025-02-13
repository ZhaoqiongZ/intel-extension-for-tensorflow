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

#ifndef ITEX_CORE_COMPILER_XLA_TYPES_H_
#define ITEX_CORE_COMPILER_XLA_TYPES_H_

#include <complex>

#include "third_party/eigen3/Eigen/Core"

namespace itex_xla {

using ::Eigen::bfloat16;
using ::Eigen::half;

using complex64 = std::complex<float>;
using complex128 = std::complex<double>;

}  // namespace itex_xla

// Alias namespace ::stream_executor as ::xla::se.
namespace stream_executor {}
namespace itex_xla {
namespace se = ::stream_executor;
}  // namespace itex_xla

#endif  // ITEX_CORE_COMPILER_XLA_TYPES_H_
