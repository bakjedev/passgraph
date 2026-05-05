#include "passgraph.hpp"
#include "pass.hpp"

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

passgraph::Pass &passgraph::Graph::add_pass(std::string name) {
  const auto id = static_cast<uint32_t>(passes_.size());
  return passes_.emplace_back(std::move(name), id);
}

std::vector<VkDependencyInfo> passgraph::Graph::compile() const {
  std::vector<VkDependencyInfo> result;

  for ([[maybe_unused]] const auto &pass : passes_) {
    result.emplace_back(VK_STRUCTURE_TYPE_DEPENDENCY_INFO);
  }

  return result;
}
