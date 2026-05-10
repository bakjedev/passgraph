#pragma once
#include <vulkan/vulkan.h>
#include "resource.hpp"

namespace passgraph {
    struct ImageResource {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        VkFormat format;
        VkImageUsageFlags usage;
    };
} // namespace passgraph
