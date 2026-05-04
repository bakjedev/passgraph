#include "passgraph.hpp"
#include "pass.hpp"

std::optional<passgraph::Graph::ResourceID> passgraph::Graph::ImportImage(VkImage image, VkFormat initial_format) {
  if (image == VK_NULL_HANDLE) {
    return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(images_.size());
  images_.emplace_back(image, initial_format);
  return id;
}

std::optional<passgraph::Graph::ResourceID>
passgraph::Graph::ImportBuffer(VkBuffer buffer, BufferLayout initial_layout) {
  if (buffer == VK_NULL_HANDLE) {
    return std::nullopt;
  }
  const auto id = static_cast<ResourceID>(buffers_.size());
  buffers_.emplace_back(buffer, initial_layout);
  return id;
}

passgraph::Pass &passgraph::Graph::AddPass(std::string name) {
  auto itr = pass_to_index_.find(name);
  if (itr != pass_to_index_.end()) {
    return passes_[itr->second];
  }
  pass_to_index_.emplace(std::move(name), static_cast<uint32_t>(passes_.size()));
  return passes_.emplace_back();
}

int passgraph::Graph::Compile() const {
  return 0;
}
