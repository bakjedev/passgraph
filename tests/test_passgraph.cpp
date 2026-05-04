#include <gtest/gtest.h>
#include "passgraph.hpp"
#include "pass.hpp"

TEST(Passgraph, SimpleTest) {
  passgraph::Graph graph;
  const auto buf = graph.ImportBuffer(nullptr, BufferLayout::Compute | BufferLayout::Vertex);
  const auto img = graph.ImportImage(nullptr);
  [[maybe_unused]] passgraph::Pass &pass = graph.AddPass("Render");
  EXPECT_EQ(graph.Compile(), 0);
  EXPECT_EQ(buf, std::nullopt);
  EXPECT_EQ(img, std::nullopt);
}
