#pragma once
#include <cstdint>
#include <functional>

#include "types/resource.hpp"

namespace passgraph {
  class Graph;
  struct Pass;

  class PassBuilder {
  public:
    explicit PassBuilder(Pass *pass, Graph *graph, size_t id);

    PassBuilder &add_color_output(ResourceID resource);

    PassBuilder &add_storage_input(ResourceID resource);

    PassBuilder &add_storage_output(ResourceID resource);

    PassBuilder &execute(std::function<void()> func);

  private:
    Pass *pass_;
    Graph *graph_;
    uint32_t id_;
  };
} // namespace passgraph
