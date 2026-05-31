#include "pass_builder.hpp"
#include "graph.hpp"
#include "types/pass.hpp"

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_color_attachment(const AttachmentInfo& info)
{
  auto& res = graph_->resource_infos_[info.resource];
  res.write_passes.insert(id_);

  ImageAccess& image = pass_->images.emplace_back(
      info.resource, info.pass ? info.pass : res.last_writer,
      Attachment{.load_op = static_cast<VkAttachmentLoadOp>(info.load_op),
                 .store_op = static_cast<VkAttachmentStoreOp>(info.store_op),
                 .clear_value = info.clear_value,
                 .is_depth = false},
      VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

  if (info.load_op == LoadOp::Load && res.last_writer.has_value()) {
    res.read_passes.insert(id_);
    image.access |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
  }
  res.last_writer = id_;

  return *this;
}

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_depth_attachment(const AttachmentInfo& info)
{
  auto& res = graph_->resource_infos_[info.resource];
  res.write_passes.insert(id_);

  ImageAccess& image = pass_->images.emplace_back(
      info.resource, info.pass ? info.pass : res.last_writer,
      Attachment{.load_op = static_cast<VkAttachmentLoadOp>(info.load_op),
                 .store_op = static_cast<VkAttachmentStoreOp>(info.store_op),
                 .clear_value = info.clear_value,
                 .is_depth = true},
      VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  if (info.load_op == LoadOp::Load && res.last_writer.has_value()) {
    res.read_passes.insert(id_);
    image.access |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
  }
  res.last_writer = id_;

  return *this;
}

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_vertex_buffer_input(const ResourceID resource)
{
  set_buffer_input(resource, VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT);
  return *this;
}

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_index_buffer_input(const ResourceID resource)
{
  set_buffer_input(resource, VK_ACCESS_2_INDEX_READ_BIT, VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT);
  return *this;
}

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_indirect_buffer_input(const ResourceID resource)
{
  set_buffer_input(resource, VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT, VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT);
  return *this;
}

passgraph::GraphicsPassBuilder& passgraph::GraphicsPassBuilder::set_render_area(const RenderArea area)
{
  pass_->render_area = area;
  return *this;
}

template<typename T>
T& passgraph::PassBuilder<T>::execute(std::function<void(VkCommandBuffer)> func)
{
  pass_->func = std::move(func);
  return static_cast<T&>(*this);
}

template<typename T>
void passgraph::PassBuilder<T>::set_buffer_input(ResourceID resource, VkAccessFlags2 access,
                                                 VkPipelineStageFlags2 stage) const
{
  auto& res = graph_->resource_infos_[resource];
  res.read_passes.insert(id_);

  pass_->buffers.emplace_back(resource, std::nullopt, access, stage);
}

template class passgraph::PassBuilder<passgraph::GraphicsPassBuilder>;
