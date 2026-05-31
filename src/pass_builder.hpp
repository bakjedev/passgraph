#pragma once
#include <functional>
#include "types/pass.hpp"
#include "types/resource.hpp"

namespace passgraph {
  class Graph;

  enum class LoadOp { Load = 0, Clear = 1, DontCare = 2 };
  enum class StoreOp { Store = 0, DontCare = 1 };

  struct AttachmentInfo {
    ResourceID resource;
    LoadOp load_op = LoadOp::DontCare;
    StoreOp store_op = StoreOp::DontCare;
    ClearValue clear_value{};
    std::optional<uint32_t> pass = std::nullopt;
  };

  template<typename T>
  class PassBuilder {
  public:
    explicit PassBuilder(Pass* pass, Graph* graph, const size_t id) :
        pass_(pass), graph_(graph), id_(static_cast<uint32_t>(id))
    {
    }

    T& execute(std::function<void(VkCommandBuffer)> func);
    [[nodiscard]] uint32_t id() const { return id_; }

  protected:
    Pass* pass_;
    Graph* graph_;
    uint32_t id_;

    void set_buffer_input(ResourceID resource, VkAccessFlags2 access, VkPipelineStageFlags2 stage) const;
  };

  class GraphicsPassBuilder : public PassBuilder<GraphicsPassBuilder> {
  public:
    explicit GraphicsPassBuilder(Pass* pass, Graph* graph, const size_t id) : PassBuilder(pass, graph, id) {}

    GraphicsPassBuilder& set_color_attachment(const AttachmentInfo& info);
    GraphicsPassBuilder& set_depth_attachment(const AttachmentInfo& info);

    GraphicsPassBuilder& set_vertex_buffer_input(ResourceID resource);
    GraphicsPassBuilder& set_index_buffer_input(ResourceID resource);
    GraphicsPassBuilder& set_indirect_buffer_input(ResourceID resource);

    GraphicsPassBuilder& set_render_area(RenderArea area);
  };
} // namespace passgraph
