#include <gtest/gtest.h>
#include "passgraph.hpp"
#include "pass.hpp"

TEST(Passgraph, SimpleTest) {
    passgraph::Graph graph;

    const auto buf = graph.import_buffer(nullptr, {.initial_layout = BufferLayout::Compute | BufferLayout::Vertex});

    EXPECT_TRUE(buf.has_value());

    graph.add_pass("Render").read(*buf);

    const auto barriers = graph.compile();

    EXPECT_EQ(barriers.size(), 1);
    EXPECT_EQ(barriers.begin()->first, "Render");
}
