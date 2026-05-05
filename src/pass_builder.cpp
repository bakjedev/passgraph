#include "pass_builder.hpp"
#include "pass_graph.hpp"

passgraph::PassBuilder::PassBuilder(Graph *graph, std::string name,
                                    const uint32_t id)
    : graph_(graph), id_(id) {
  pass_.name_ = std::move(name);
}

passgraph::PassBuilder &passgraph::PassBuilder::read(uint32_t resource_id) {
  pass_.dependencies_.insert(resource_id);
  return *this;
}

uint32_t passgraph::PassBuilder::build() const {
  graph_->insert_pass(pass_);
  return id_;
}