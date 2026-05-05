#pragma once
#include <cstdint>
#include <string>
#include <unordered_set>

namespace passgraph {
class Pass {
public:
  Pass(std::string name, uint32_t id);
  
  Pass &read(uint32_t resource_id);

  uint32_t id() const { return id_; }
  const std::string &get_name() const { return name_; }

private:
  std::unordered_set<uint32_t> dependencies_;
  std::string name_;
  uint32_t id_;
};
} // namespace passgraph
