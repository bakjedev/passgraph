#pragma once
#include "types/pass.hpp"
#include <cstdint>
#include <string>

namespace passgraph {
class Graph;

class PassBuilder {
public:
  PassBuilder(Graph *graph, std::string name, uint32_t id);

  PassBuilder &read(uint32_t resource_id);

  uint32_t build() const;

private:
  Graph *graph_ = nullptr;
  uint32_t id_;

  Pass pass_;
};

} // namespace passgraph