#include <gtest/gtest.h>
#include "context.hpp"

TEST(Passgraph, SimpleTest)
{
  passgraph::Context context;

  const auto buf =
      context.import_buffer({.size = 0, .usage = 0u, .state = passgraph::BufferState::Undefined}, nullptr, "Data");

  const auto img = context.import_image({.x = 1920,
                                         .y = 1080,
                                         .z = 0,
                                         .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                                         .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                         .aspect = VK_IMAGE_ASPECT_COLOR_BIT,
                                         .state = passgraph::ImageState::Undefined},
                                        nullptr, nullptr, "RenderTarget");

  EXPECT_TRUE(buf);
  EXPECT_TRUE(img);

  passgraph::Graph& graph = context.graph();

  const uint32_t first = graph.add_graphics_pass("First")
                             .set_color_attachment({.resource = {img}})
                             .set_execute([](VkCommandBuffer) { std::cout << "A" << "\n"; })
                             .id();

  graph.add_graphics_pass("Second").set_color_attachment({.resource = {img}}).set_execute([](VkCommandBuffer) {
    std::cout << "B" << "\n";
  });

  graph.add_graphics_pass("Third").set_texture_input({.resource = {img, first}}).set_execute([](VkCommandBuffer) {
    std::cout << "C" << "\n";
  });

  EXPECT_TRUE(graph.compile());

  graph.execute(nullptr);
}
