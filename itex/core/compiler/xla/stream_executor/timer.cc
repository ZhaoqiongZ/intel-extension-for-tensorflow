/* Copyright (c) 2023 Intel Corporation

Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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

#include "itex/core/compiler/xla/stream_executor/timer.h"

#include "itex/core/compiler/xla/stream_executor/platform.h"
#include "itex/core/compiler/xla/stream_executor/platform/logging.h"
#include "itex/core/compiler/xla/stream_executor/platform/port.h"
#include "itex/core/compiler/xla/stream_executor/stream_executor.h"

namespace stream_executor {

Timer::Timer(StreamExecutor* parent)
    : parent_(parent),
      implementation_(parent_->implementation()->GetTimerImplementation()) {}

Timer::~Timer() { parent_->DeallocateTimer(this); }

uint64_t Timer::Microseconds() const { return implementation_->Microseconds(); }

uint64_t Timer::Nanoseconds() const { return implementation_->Nanoseconds(); }

}  // namespace stream_executor
