#pragma once

template<fwrk::ImageInterface I>
fwrk::ResourceID fwrk::Context::import_image(const I& image, const ImageState& state, std::string name)
{
  const auto slot_id = images_.size();

  images_.push_back(construct_image(image, state));
  views_cache_.emplace_back();

  const auto raw_id = raw_images_.size();
  raw_images_.push_back(image.image());

  const auto id = resources_.size();
  resources_.emplace_back(ResourceType::Image, slot_id, raw_id, std::move(name));

  return ResourceID{id};
}

template<fwrk::BufferInterface I>
fwrk::ResourceID fwrk::Context::import_buffer(const I& buffer, const BufferState& state, std::string name)
{
  const auto slot_id = buffers_.size();
  buffers_.emplace_back(buffer.size(), buffer.usage(), state);

  const auto raw_id = raw_buffers_.size();
  raw_buffers_.push_back(buffer.buffer());

  const auto id = resources_.size();
  resources_.emplace_back(ResourceType::Buffer, slot_id, raw_id, std::move(name));

  return ResourceID{id};
}

template<fwrk::ImageInterface I>
void fwrk::Context::update_image(const ResourceID resource, const I& image, const ImageState& state)
{
  if (!resource) return;
  const Resource& res = resources_[*resource.id];
  ImageResource& img = images_[res.slot];

  destroy_views(res.slot);
  img = construct_image(image, state);
  raw_images_[res.raw] = image.image();
}

template<fwrk::BufferInterface I>
void fwrk::Context::update_buffer(const ResourceID resource, const I& buffer, const BufferState& state)
{
  if (!resource) return;
  const Resource& res = resources_[*resource.id];
  BufferResource& buf = buffers_[res.slot];

  buf = BufferResource{.size = buffer.size(), .usage = buffer.usage(), .state = state};
  raw_buffers_[res.raw] = buffer.buffer();
}

template<fwrk::ImageInterface I>
fwrk::ImageResource fwrk::Context::construct_image(const I& image, const ImageState& state) const
{
  uint32_t x = 0, y = 0, z = 0;
  if constexpr (requires {
                  image.x();
                  image.y();
                  image.z();
                }) {
    x = image.x();
    y = image.y();
    z = image.z();
  } else {
    const auto [size_x, size_y, size_z] = image.size();
    x = size_x;
    y = size_y;
    z = size_z;
  }
  return ImageResource{.x = x,
                       .y = y,
                       .z = z,
                       .format = image.format(),
                       .usage = image.usage(),
                       .aspect = image.aspect(),
                       .layer_count = image.layer_count(),
                       .level_count = image.level_count(),
                       .state = state};
}
