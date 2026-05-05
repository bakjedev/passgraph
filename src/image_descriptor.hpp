#pragma once
#include <vulkan/vulkan.h>

namespace passgraph {
    struct ImageDescriptor {
        VkFormat format;
        VkImageUsageFlags usage;
        VkImageLayout initial_layout;
    };
} // namespace passgraph
