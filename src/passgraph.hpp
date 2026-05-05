#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>
#include <optional>

#include "buffer_descriptor.hpp"
#include "image_descriptor.hpp"


namespace passgraph {
  class Pass;

  class Graph {
  public:
    using ResourceID = uint32_t;

    [[nodiscard]] std::optional<ResourceID> import_image(VkImage image, const ImageDescriptor &desc);

    [[nodiscard]] std::optional<ResourceID> import_buffer(VkBuffer buffer, const BufferDescriptor &desc);

    [[nodiscard]] Pass &add_pass(std::string name);

    [[nodiscard]] std::unordered_map<std::string, VkDependencyInfo> compile() const;

  private:
    using Image = std::pair<VkImage, ImageDescriptor>;
    using Buffer = std::pair<VkBuffer, BufferDescriptor>;

    std::vector<Image> images_;
    std::vector<Buffer> buffers_;

    std::unordered_map<std::string, Pass> passes_;
  };
} // namespace passgraph
