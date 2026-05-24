#include <gtest/gtest.h>
#include "pass_graph.hpp"

TEST(Passgraph, SimpleTest)
{
  passgraph::Graph graph;

  const auto buf = graph.import_buffer(
      "Data",
      {.size = 0, .usage = 0u, .initial_state = {.access = VK_ACCESS_2_NONE, .stage = VK_PIPELINE_STAGE_2_NONE}},
      nullptr);

  const auto img = graph.import_image("RenderTarget",
                                      {.x = 1920,
                                       .y = 1080,
                                       .z = 0,
                                       .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                                       .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                       .initial_state = {.access = VK_ACCESS_2_NONE,
                                                         .stage = VK_PIPELINE_STAGE_2_NONE,
                                                         .layout = VK_IMAGE_LAYOUT_UNDEFINED}},
                                      nullptr);

  EXPECT_TRUE(buf);
  EXPECT_TRUE(img);

  graph.add_pass("First", passgraph::QueueFlags::Graphics)
      .add_color_attachment({.resource = img})
      .execute([](VkCommandBuffer) { std::cout << "A" << "\n"; });

  graph.add_pass("Second", passgraph::QueueFlags::Graphics)
      .add_color_attachment({.resource = img, .load_op = passgraph::LoadOp::Load})
      .execute([](VkCommandBuffer) { std::cout << "B" << "\n"; });

  EXPECT_TRUE(graph.compile());

  graph.execute(nullptr);
}
