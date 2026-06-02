#pragma once
#include <vulkan/vulkan.h>

namespace passgraph {
  struct ImageState {
    VkAccessFlags2 access;
    VkPipelineStageFlags2 stage;
    VkImageLayout layout;

    static const ImageState Undefined;

    bool operator==(const ImageState& other) const = default;
  };
  constexpr ImageState ImageState::Undefined{VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_NONE, VK_IMAGE_LAYOUT_UNDEFINED};

  struct ImageResource {
    uint32_t x;
    uint32_t y;
    uint32_t z;
    VkFormat format;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspect;
    uint32_t layer_count;
    uint32_t level_count;

    ImageState state;
  };

  template<typename T>
  concept ImageInterface =
    (requires(T img) {
      { img.x() } -> std::same_as<uint32_t>;
      { img.y() } -> std::same_as<uint32_t>;
      { img.z() } -> std::same_as<uint32_t>;
    } || requires(T img) {
      { img.size() } -> std::convertible_to<std::tuple<uint32_t, uint32_t, uint32_t>>;
    }) && requires(T img) {
      { img.format() } -> std::same_as<VkFormat>;
      { img.usage() } -> std::same_as<VkImageUsageFlags>;
      { img.aspect() } -> std::same_as<VkImageAspectFlags>;
      { img.layer_count() } -> std::same_as<uint32_t>;
      { img.level_count() } -> std::same_as<uint32_t>;
      { img.image() } -> std::same_as<VkImage>;
    };
} // namespace passgraph
