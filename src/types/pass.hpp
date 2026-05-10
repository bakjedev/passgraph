#pragma once
#include <functional>
#include <string>
#include <vector>

namespace passgraph {
  struct Pass {
    std::string name;
    std::function<void()> func;
    std::vector<Resource> dependencies;
  };
} // namespace passgraph
