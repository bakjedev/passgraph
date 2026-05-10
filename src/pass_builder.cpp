#include "pass_builder.hpp"
#include "types/pass.hpp"

passgraph::PassBuilder::PassBuilder(Pass *pass) : pass_(pass) {
}

passgraph::PassBuilder &passgraph::PassBuilder::read([[maybe_unused]] const uint32_t resource_id) {
  return *this;
}

passgraph::PassBuilder &passgraph::PassBuilder::execute(std::function<void()> func) {
  pass_->func_ = std::move(func);
  return *this;
}
