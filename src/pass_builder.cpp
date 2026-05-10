#include "pass_builder.hpp"
#include "types/pass.hpp"
#include "pass_graph.hpp"

passgraph::PassBuilder::PassBuilder(Pass *pass, Graph *graph, size_t id) : pass_(pass), graph_(graph),
                                                                           id_(static_cast<uint32_t>(id)) {
}

passgraph::PassBuilder &passgraph::PassBuilder::add_color_output(const ResourceID resource) {
  auto &res = graph_->resources_[resource.id];
  res.write_passes.push_back(id_);
  return *this;
}

passgraph::PassBuilder &passgraph::PassBuilder::add_storage_input(ResourceID resource) {
  auto &res = graph_->resources_[resource.id];
  res.read_passes.push_back(id_);
  return *this;
}

passgraph::PassBuilder &passgraph::PassBuilder::add_storage_output(ResourceID resource) {
  auto &res = graph_->resources_[resource.id];
  res.write_passes.push_back(id_);
  return *this;
}

passgraph::PassBuilder &passgraph::PassBuilder::execute(std::function<void()> func) {
  pass_->func = std::move(func);
  return *this;
}
