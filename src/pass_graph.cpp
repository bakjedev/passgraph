#include "pass_graph.hpp"
#include "types/pass.hpp"

std::optional<passgraph::Graph::ResourceID>
passgraph::Graph::import_image(VkImage image, const ImageDescriptor &desc) {
  if (image == VK_NULL_HANDLE) {
    // return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(images_.size());
  images_.emplace_back(image, desc);
  return id;
}

std::optional<passgraph::Graph::ResourceID>
passgraph::Graph::import_buffer(VkBuffer buffer, const BufferDescriptor &desc) {
  if (buffer == VK_NULL_HANDLE) {
    // return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(buffers_.size());
  buffers_.emplace_back(buffer, desc);
  return id;
}

passgraph::PassBuilder passgraph::Graph::add_pass(std::string name) {
  passes_.emplace_back().name_ = std::move(name);
  return PassBuilder{&passes_.back()};
}

bool passgraph::Graph::compile() const {
  return true;
}

void passgraph::Graph::execute() const {
  for (const Pass &pass: passes_) {
    // insert pass.barrier whatever
    pass.func_();
  }
}
