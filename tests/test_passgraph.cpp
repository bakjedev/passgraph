#include <gtest/gtest.h>
#include "passgraph.hpp"
#include "pass.hpp"

TEST(Passgraph, SimpleTest) {
  PassGraph graph;
  Pass& pass = graph.AddPass("Render");
  pass.AddResource("texture");
  EXPECT_EQ(graph.Compile(), 0);
}
