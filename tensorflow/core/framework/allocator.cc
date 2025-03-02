/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/core/framework/allocator.h"

#include <atomic>

#include "tensorflow/core/framework/allocator_registry.h"
#include "tensorflow/core/framework/tracking_allocator.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/mem.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/types.h"

namespace tensorflow {

string AllocatorStats::DebugString() const {
  return strings::Printf(
      "Limit:        %20lld\n"
      "InUse:        %20lld\n"
      "MaxInUse:     %20lld\n"
      "NumAllocs:    %20lld\n"
      "MaxAllocSize: %20lld\n",
      this->bytes_limit ? *this->bytes_limit : 0, this->bytes_in_use,
      this->peak_bytes_in_use, this->num_allocs, this->largest_alloc_size);
}

constexpr size_t Allocator::kAllocatorAlignment;

Allocator::~Allocator() {}

// If true, cpu allocator collects full stats.
static bool cpu_allocator_collect_full_stats = false;

void EnableCPUAllocatorFullStats(bool enable) {
  cpu_allocator_collect_full_stats = enable;
}
bool CPUAllocatorFullStatsEnabled() { return cpu_allocator_collect_full_stats; }

string AllocatorAttributes::DebugString() const {
  return strings::StrCat("AllocatorAttributes(on_host=", on_host(),
                         " nic_compatible=", nic_compatible(),
                         " gpu_compatible=", gpu_compatible(), ")");
}

Allocator* cpu_allocator_base() {
  static Allocator* cpu_alloc =
      AllocatorFactoryRegistry::singleton()->GetAllocator();
  // TODO(tucker): This really seems wrong.  It's only going to be effective on
  // the first call in a process (but the desired effect is associated with a
  // session), and we probably ought to be tracking the highest level Allocator,
  // not the lowest.  Revisit the advertised semantics of the triggering option.
  if (cpu_allocator_collect_full_stats && !cpu_alloc->TracksAllocationSizes()) {
    cpu_alloc = new TrackingAllocator(cpu_alloc, true);
  }
  return cpu_alloc;
}

Allocator* cpu_allocator(int numa_node) {
  // Correctness relies on devices being created prior to the first call
  // to cpu_allocator, if devices are ever to be created in the process.
  // Device creation in turn triggers ProcessState creation and the availability
  // of the correct access pointer via this function call.
  static ProcessStateInterface* ps =
      AllocatorFactoryRegistry::singleton()->process_state();
  if (ps) {
    return ps->GetCPUAllocator(numa_node);
  } else {
    return cpu_allocator_base();
  }
}

Allocator* pmem_allocator() {
  static Allocator* pmem_alloc =
      AllocatorFactoryRegistry::singleton()->GetPMEMAllocator();
      //This is the function when we use pmem as allocation destination
  if (pmem_alloc && cpu_allocator_collect_full_stats && !pmem_alloc->TracksAllocationSizes()) {
      pmem_alloc = new TrackingAllocator(pmem_alloc, true);
  }
  return pmem_alloc;
}

Allocator* ev_allocator() {
  static Allocator* ev_alloc =
      AllocatorFactoryRegistry::singleton()->GetEVAllocator();
      //This is the function when we use ev as allocation destination
  if (ev_alloc && cpu_allocator_collect_full_stats && !ev_alloc->TracksAllocationSizes()) {
      ev_alloc = new TrackingAllocator(ev_alloc, true);
  }
  return ev_alloc;
}

SubAllocator::SubAllocator(const std::vector<Visitor>& alloc_visitors,
                           const std::vector<Visitor>& free_visitors)
    : alloc_visitors_(alloc_visitors), free_visitors_(free_visitors) {}

void SubAllocator::VisitAlloc(void* ptr, int index, size_t num_bytes) {
  for (const auto& v : alloc_visitors_) {
    v(ptr, index, num_bytes);
  }
}

void SubAllocator::VisitFree(void* ptr, int index, size_t num_bytes) {
  // Although we don't guarantee any order of visitor application, strive
  // to apply free visitors in reverse order of alloc visitors.
  for (int i = free_visitors_.size() - 1; i >= 0; --i) {
    free_visitors_[i](ptr, index, num_bytes);
  }
}
}  // namespace tensorflow
