#include "pass_graph.hpp"
#include <gtest/gtest.h>

TEST(Passgraph, SimpleTest) {
    passgraph::Graph graph;

    const auto buf =
            graph.import_buffer(nullptr, {
                                    .initial_layout = BufferLayout::Compute |
                                                      BufferLayout::Vertex
                                });

    EXPECT_TRUE(buf.has_value());

    graph.add_pass("Render").read(*buf).execute([] {
        std::cout << "YEAH" << "\n";
    });

    EXPECT_TRUE(graph.compile());

    graph.execute();
}
