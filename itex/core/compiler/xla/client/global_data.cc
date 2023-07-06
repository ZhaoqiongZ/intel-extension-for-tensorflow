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

#include "itex/core/compiler/xla/client/global_data.h"

#include <string>
#include <utility>

#include "absl/container/flat_hash_map.h"
#include "itex/core/compiler/xla/types.h"
#include "itex/core/utils/logging.h"

namespace itex_xla {
namespace {

// Releases a set of global data handles owned by the parent service
// interface.
void ReleaseHandles(ServiceInterface* parent,
                    const absl::Span<const GlobalDataHandle> handles) {
  UnregisterRequest request;
  for (auto& handle : handles) {
    ITEX_VLOG(1) << "Requesting to unregister " << handle.ShortDebugString();
    *request.add_data() = handle;
  }
  UnregisterResponse response;
  Status status = parent->Unregister(&request, &response);
  ITEX_VLOG(1) << "Done with request";
  if (!status.ok()) {
    ITEX_LOG(WARNING) << "Failed to unregister handles: " << status
                      << "; continuing anyway...";
  }
}

}  // namespace

GlobalData::GlobalData(ServiceInterface* parent, GlobalDataHandle handle)
    : handle_(std::move(handle)), parent_(parent) {}

GlobalData::~GlobalData() {
  if (parent_ != nullptr) {
    ReleaseHandles(parent_, {handle_});
  }
}

/* static */ void GlobalData::Release(
    std::vector<std::unique_ptr<GlobalData>> instances) {
  absl::flat_hash_map<ServiceInterface*, std::vector<GlobalDataHandle>>
      parent_handles_map;
  for (auto& instance : instances) {
    if (instance->parent_ != nullptr) {
      parent_handles_map[instance->parent_].push_back(instance->Release());
    }
  }
  for (auto& parent_handles : parent_handles_map) {
    ReleaseHandles(parent_handles.first, parent_handles.second);
  }
}

}  // namespace itex_xla
