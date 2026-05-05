#pragma once
#include <cstdint>
#include <unordered_set>

namespace passgraph {
  class Pass {
  public:
    Pass &read(uint32_t resource_id);

  private:
    friend class Graph;

    std::unordered_set<uint32_t> dependencies_;
  };
} // namespace passgraph
