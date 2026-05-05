#pragma once
#include <cstdint>

enum class BufferLayout : uint32_t {
    Undefined = 1 << 0,
    Vertex = 1 << 1,
    Fragment = 1 << 2,
    Compute = 1 << 3,
};

constexpr BufferLayout operator|(BufferLayout lhs, BufferLayout rhs) {
    return static_cast<BufferLayout>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr bool operator&(BufferLayout lhs, BufferLayout rhs) {
    return static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs);
}
