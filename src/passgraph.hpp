#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>
#include <optional>
#include "buffer_layout.hpp"

namespace passgraph {
  class Pass;

  class Graph {
  public:
    using ResourceID = uint32_t;

    [[nodiscard]] std::optional<ResourceID> ImportImage(VkImage image, VkFormat initial_format = VK_FORMAT_UNDEFINED);

    [[nodiscard]] std::optional<ResourceID> ImportBuffer(VkBuffer buffer,
                                                         BufferLayout initial_layout = BufferLayout::Undefined);

    [[nodiscard]] Pass &AddPass(std::string name);

    int Compile() const;

  private:
    std::vector<Pass> passes_;
    std::unordered_map<std::string, uint32_t> pass_to_index_;

    struct ImageInfo {
      VkImage image;
      VkFormat initial_format;
    };

    struct BufferInfo {
      VkBuffer buffer;
      BufferLayout initial_layout;
    };

    std::vector<ImageInfo> images_;
    std::vector<BufferInfo> buffers_;
  };
} // namespace passgraph
