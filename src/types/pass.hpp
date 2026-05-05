#pragma once
#include <cstdint>
#include <string>
#include <unordered_set>

namespace passgraph {
struct Pass {
  std::unordered_set<uint32_t> dependencies_;
  std::string name_;
};
} // namespace passgraph
