#pragma once
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "pass_builder.hpp"
#include "types/buffer_descriptor.hpp"
#include "types/image_descriptor.hpp"
#include "types/pass.hpp"

namespace passgraph {

class Graph {
public:
  using ResourceID = uint32_t;

  [[nodiscard]] std::optional<ResourceID>
  import_image(VkImage image, const ImageDescriptor &desc);

  [[nodiscard]] std::optional<ResourceID>
  import_buffer(VkBuffer buffer, const BufferDescriptor &desc);

  [[nodiscard]] PassBuilder add_pass(std::string name);

  [[nodiscard]] std::vector<VkDependencyInfo> compile() const;

private:
  friend class PassBuilder;

  using Image = std::pair<VkImage, ImageDescriptor>;
  using Buffer = std::pair<VkBuffer, BufferDescriptor>;

  std::vector<Image> images_;
  std::vector<Buffer> buffers_;

  std::vector<Pass> passes_;
  void insert_pass(Pass pass);
};
} // namespace passgraph
