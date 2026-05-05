#include "pass.hpp"

passgraph::Pass::Pass(std::string name, const uint32_t id)
    : name_(std::move(name)), id_(id) {}

passgraph::Pass &passgraph::Pass::read(const uint32_t resource_id) {
  dependencies_.insert(resource_id);
  return *this;
}
