#pragma once

namespace passgraph {
  struct BufferResource {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
  };
} // namespace passgraph
