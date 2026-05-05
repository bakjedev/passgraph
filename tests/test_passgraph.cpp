#include "pass.hpp"
#include "passgraph.hpp"
#include <gtest/gtest.h>

TEST(Passgraph, SimpleTest) {
  passgraph::Graph graph;

  const auto buf =
      graph.import_buffer(nullptr, {.initial_layout = BufferLayout::Compute |
                                                      BufferLayout::Vertex});

  EXPECT_TRUE(buf.has_value());

  const auto render_pass = graph.add_pass("Render").read(*buf).id();

  const auto barriers = graph.compile();

  EXPECT_EQ(barriers.size(), 1);
  EXPECT_EQ(render_pass, 0);
  EXPECT_EQ(barriers[render_pass].sType, VK_STRUCTURE_TYPE_DEPENDENCY_INFO);
}
