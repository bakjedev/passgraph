# WIP, not done and unready.

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
