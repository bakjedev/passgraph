#include "passgraph.hpp"
#include "pass.hpp"

std::optional<passgraph::Graph::ResourceID> passgraph::Graph::import_image(VkImage image, const ImageDescriptor &desc) {
  if (image == VK_NULL_HANDLE) {
    //return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(images_.size());
  images_.emplace_back(image, desc);
  return id;
}

std::optional<passgraph::Graph::ResourceID>
passgraph::Graph::import_buffer(VkBuffer buffer, const BufferDescriptor &desc) {
  if (buffer == VK_NULL_HANDLE) {
    //return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(buffers_.size());
  buffers_.emplace_back(buffer, desc);
  return id;
}

passgraph::Pass &passgraph::Graph::add_pass(std::string name) {
  auto itr = passes_.find(name);
  if (itr != passes_.end()) {
    return itr->second;
  }
  return passes_[std::move(name)];
}

std::unordered_map<std::string, VkDependencyInfo> passgraph::Graph::compile() const {
  std::unordered_map<std::string, VkDependencyInfo> result;

  for (const auto &[name, pass]: passes_) {
    result.emplace(name, VkDependencyInfo{});
  }

  return result;
}

