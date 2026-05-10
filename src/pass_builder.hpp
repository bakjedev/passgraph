#pragma once
#include <cstdint>
#include <functional>

namespace passgraph {
  struct Pass;

  class PassBuilder {
  public:
    explicit PassBuilder(Pass *pass);

    PassBuilder &read(uint32_t resource_id);

    PassBuilder &execute(std::function<void()> func);

  private:
    Pass *pass_;
  };
} // namespace passgraph
