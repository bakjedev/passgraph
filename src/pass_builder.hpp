#pragma once
#include <cstdint>
#include <functional>

#include "types/pass.hpp"
#include "types/resource.hpp"

namespace passgraph {
  class Graph;

  enum class LoadOp { Load, Clear, DontCare };

  struct ColorAttachmentInfo {
    ResourceID resource;
    LoadOp load_op = LoadOp::DontCare;
    std::optional<uint32_t> pass = std::nullopt;
  };

  class PassBuilder {
  public:
    explicit PassBuilder(Pass* pass, Graph* graph, size_t id);

    PassBuilder& add_color_attachment(const ColorAttachmentInfo& info);

    PassBuilder& execute(std::function<void()> func);

    [[nodiscard]] uint32_t id() const { return id_; }

  private:
    Pass* pass_;
    Graph* graph_;
    uint32_t id_;
  };
} // namespace passgraph
