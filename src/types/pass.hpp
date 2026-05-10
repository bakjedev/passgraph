#pragma once
#include <functional>
#include <string>

namespace passgraph {
  struct Pass {
    std::string name_;
    std::function<void()> func_;
  };
} // namespace passgraph
