#include <gtest/gtest.h>
#include "passgraph.hpp"

TEST(Passgraph, SimpleTest) {
  PassGraph graph;
  EXPECT_EQ(graph.GetAValue(), 0);
}
