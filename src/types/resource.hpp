#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

namespace passgraph {
    enum class ResourceType : uint8_t {
        Image,
        Buffer,
    };

    struct ResourceID {
        static constexpr uint32_t Invalid = ~0u;

        uint32_t id = Invalid;

        ResourceID() = default;

        explicit ResourceID(const size_t id_) : id(static_cast<uint32_t>(id_)) {
        }

        explicit operator bool() const { return id != Invalid; }
    };

    struct Resource {
        ResourceType type;
        uint32_t slot;
        uint32_t raw;
        uint32_t generation = 0;
        std::string name;
        std::vector<uint32_t> write_passes;
        std::vector<uint32_t> read_passes;

        Resource(const ResourceType type_, const size_t id_, const size_t raw_, std::string name_) : type(type_),
            slot(static_cast<uint32_t>(id_)), raw(static_cast<uint32_t>(raw_)), name(std::move(name_)) {
        }
    };
}
