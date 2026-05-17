#pragma once
#include <vulkan/vulkan.h>

namespace passgraph {
  struct BufferResource {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
  };
} // namespace passgraph
