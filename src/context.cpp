#include "context.hpp"

fwrk::Context::~Context()
{
  if (!device_) return;
  for (auto& views: views_cache_) {
    for (auto [_, view]: views) {
      if (view) {
        vkDestroyImageView(device_, view, nullptr);
      }
    }
  }
}

fwrk::ResourceID fwrk::Context::import_image(const ImageResource& image, VkImage raw, std::string name)
{
  if (raw == VK_NULL_HANDLE) {
    // return ResourceID{};
  }

  const auto slot_id = images_.size();
  images_.push_back(image);
  views_cache_.emplace_back();

  const auto raw_id = raw_images_.size();
  raw_images_.push_back(raw);

  const auto id = resources_.size();
  resources_.emplace_back(ResourceType::Image, slot_id, raw_id, std::move(name));

  return ResourceID{id};
}

fwrk::ResourceID fwrk::Context::import_buffer(const BufferResource& buffer, VkBuffer raw, std::string name)
{
  if (raw == VK_NULL_HANDLE) {
    // return ResourceID{};
  }

  const auto slot_id = buffers_.size();
  buffers_.push_back(buffer);

  const auto raw_id = raw_buffers_.size();
  raw_buffers_.push_back(raw);

  const auto id = resources_.size();
  resources_.emplace_back(ResourceType::Buffer, slot_id, raw_id, std::move(name));

  return ResourceID{id};
}

VkImageView fwrk::Context::get_image_view(const ImageAccess& image_access, const Resource& resource)
{
  if (device_ == VK_NULL_HANDLE) return VK_NULL_HANDLE;
  VkImage image_raw = raw_images_[resource.raw];
  const ImageResource& image = images_[resource.slot];

  auto& views = views_cache_[resource.slot];

  const ViewKey key = {image_access.level, image.level_count, image_access.layer, image.layer_count};
  auto it = views.find(key);
  if (it != views.end()) {
    return it->second;
  }

  const VkImageViewCreateInfo view_create_info{
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0u,
      .image = image_raw,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = image.format,
      .components = {},
      .subresourceRange =
          {
              .aspectMask = image.aspect,
              .baseMipLevel = image_access.level,
              .levelCount = image.level_count,
              .baseArrayLayer = image_access.layer,
              .layerCount = image.layer_count,
          },
  };

  VkImageView view = VK_NULL_HANDLE;
  if (vkCreateImageView(device_, &view_create_info, nullptr, &view) != VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  views[key] = view;

  return view;
}
