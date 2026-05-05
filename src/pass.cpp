#include "pass.hpp"

passgraph::Pass &passgraph::Pass::read(const uint32_t resource_id) {
    dependencies_.insert(resource_id);
    return *this;
}
