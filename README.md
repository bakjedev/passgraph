# passgraph
A C++ library that takes render passes with stage/access masks per required resource, builds a
dependency graph, and creates the correct VkDependencyInfo structs to insert barriers for each
pass. Using `VK_KHR_synchronization2`.
