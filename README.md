# passgraph
A C++ library that takes render passes with stage/access masks per required resource, builds a
dependency graph, and creates the correct VkDependencyInfo structs to insert barriers for each
pass. Using `VK_KHR_synchronization2`.

## Prerequisites
- Vulkan SDK >= 1.3
- Meson >= 1.1.0
- Ninja
- C++20 compiler
- GoogleTest (not required if not running tests)

## Build
```
meson setup build
```
```
meson compile -C build
```

## Test
```
meson test -C build
```
